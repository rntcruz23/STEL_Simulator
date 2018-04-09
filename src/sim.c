#include <stdio.h>
#include <time.h>

#include "server.h"

/*      Informacao da protecao civil    */
#define L_CIVIL         15
#define p_CIVIL         0.4 
#define dmin_CIVIL      1*60
#define dmax_CIVIL      4*60
#define lamb_CIVIL      1.5*60
#define m_CIVIL         21

/*      Informacao do inem              */
#define L_INEM          m_CIVIL
#define p_INEM          0.6
#define med_INEM        45.0
#define desv_INEM       15.0
#define wait_min_INEM   30.0
#define wait_max_INEM   75.0
#define dmin_INEM       1*60
#define dmax_INEM       9999999      /* No time limit    */
#define lamb_INEM       1.5*60
#define m_INEM          100

#define l               (600.0/3600.0)
#define K               11000000

int destiny(){
    //printf("%d\n",_random() < p_CIVIL?CIVIL:INEM);
    return _random() < p_CIVIL?CIVIL:INEM;
}

int main(){
    srand(time(NULL));

    lista *events = NULL;
    server *inem = newServer(m_INEM,L_INEM,REDIRECT,INEM,lamb_INEM,dmin_INEM,dmax_INEM);
    server *civil = newServer(m_CIVIL,L_CIVIL,CHEGADA,CIVIL,lamb_CIVIL,dmin_CIVIL,dmax_CIVIL);
    double current_time = 0, redirectsum = 0;
    int etotal = 0;

	events = adicionar(events,newEvent(CHEGADA,destiny(),current_time + nextArrival(l,1)));
    etotal++;

    while(etotal < 1000000){
        event prox; 
        event atual = nextEvent(&events);

        current_time = atual.tempo;
        int tipo = atual.tipo;
        int destino = atual.destino;
        etotal++;

        if(tipo == CHEGADA){
            int serverState = pick_up(civil,atual);
            if (serverState == FREE){
                if(destino == INEM){
                    double inem_pickup = nextBoxInterval(med_INEM,desv_INEM,wait_min_INEM,wait_max_INEM);
                    redirectsum += inem_pickup;
                    prox = newEvent(REDIRECT,INEM,current_time + inem_pickup);
                }
                else if(destino == CIVIL){
                    double serviceTime = nextServiceInterval(civil->lamb,civil->dmin,civil->dmax);
                    prox = newEvent(PARTIDA,CIVIL,current_time + serviceTime);
                    addHistogramEntry(civil->hist,serviceTime);
                }
                events = adicionar(events,prox);
            }
            double temp = current_time + nextArrival(l,1);
            prox = newEvent(CHEGADA,destiny(),temp);
            events = adicionar(events,prox);
        }
        else if (tipo == REDIRECT){
            int serverState =  pick_up(inem,atual);
            if(serverState == FREE){
                double serviceTime = nextServiceInterval(inem->lamb,inem->dmin,inem->dmax);
                prox = newEvent(PARTIDA,INEM,current_time + serviceTime);
                events = adicionar(events,prox);

                civil->active--;
                addHistogramEntry(inem->hist,serviceTime);
            }
        }
        else if (tipo == PARTIDA){
            event nextServiceFromBuffer;
            nextServiceFromBuffer.tempo = -1;
            if(destino == CIVIL){
                nextServiceFromBuffer = hang_up(civil,current_time);  
            }
            else if (destino == INEM){
                nextServiceFromBuffer = hang_up(inem,current_time);
                if(nextServiceFromBuffer.tempo != -1)
                    civil->active--;
            }
            events = adicionar(events,nextServiceFromBuffer);   /* Add PARTIDA event from next event in buffer  */
        }
    }
   printf("\nEstatisticas de proteçao Civil\n");
   printServerStatus(*civil);

   printf("\nEstatisticas do INEM\n");
   printServerStatus(*inem);
   printf("\nMedia de tempo entre chegada ao sistema e atendimento pelo INEM: %f\n",inem->totalServiced?(double)redirectsum/inem->totalServiced:0.0);

   endServer(inem);
   endServer(civil);
   lista_apaga(events);
}