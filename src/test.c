#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "server.h"

/*      Informacao da protecao civil    */
#define L_CIVIL         7
#define p_CIVIL         0.4
#define dmin_CIVIL      (1*60)
#define dmax_CIVIL      (4.0*60)
#define lamb_CIVIL      (1.5*60)
#define m_CIVIL         14

/*      Informacao do inem              */
#define L_INEM          m_CIVIL
#define p_INEM          0.6
#define med_INEM        45.0
#define desv_INEM       15.0
#define wait_min_INEM   15.0
#define wait_max_INEM   75.0
#define dmin_INEM       (1*60)
#define dmax_INEM       RAND_MAX      /* No time limit    */
#define lamb_INEM       (1.5*60)
#define m_INEM          20

#define l               (600.0/3600.0)

int destiny(){
    return _random() < p_CIVIL?CIVIL:INEM;
}

int main(){
    srand(time(NULL));

    lista *events = NULL;
    server *inem = newServer(m_INEM,L_INEM,REDIRECT,INEM,lamb_INEM,dmin_INEM,dmax_INEM);
    server *civil = newServer(m_CIVIL,L_CIVIL,CHEGADA,CIVIL,lamb_CIVIL,dmin_CIVIL,dmax_CIVIL);
    histogram *gaussian = newHistogram(10000);
    double current_time = 0;
    int etotal = 0;

    etotal++;

    int civil_m,inem_m,civil_l;
    for(civil_m = m_CIVIL;civil_m <= m_CIVIL+5; civil_m++){
        for(inem_m = m_INEM;inem_m <= m_INEM+3;inem_m++){
            for(civil_l = L_CIVIL;civil_l <= L_CIVIL + 3;civil_l++){
                printf("Civil channels: %d\n",civil_m);
                printf("Inem channels: %d\n",inem_m);
                printf("Civil buffer: %d\n",civil_l);
                inem->channels = inem_m;
                civil->channels = civil_m;
                civil->bufferSize = civil_l;
                inem->bufferSize = civil_m;
                events = adicionar(events,newEvent(CHEGADA,destiny(),current_time + nextArrival(l,1)));
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
                                prox = newEvent(REDIRECT,INEM,current_time + inem_pickup);

                                addHistogramEntry(gaussian,inem_pickup);
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
                            if(nextServiceFromBuffer.tempo != -1){
                                if (nextServiceFromBuffer.destino == CIVIL){
                                    nextServiceFromBuffer.tipo = PARTIDA;
                                    nextServiceFromBuffer.tempo =  current_time + nextServiceInterval(civil->lamb,civil->dmin,civil->dmax);
                                }
                                else if (nextServiceFromBuffer.destino == INEM){
                                    nextServiceFromBuffer.tipo = REDIRECT;
                                    double temp = nextBoxInterval(med_INEM,desv_INEM,wait_min_INEM,wait_max_INEM);
                                    addHistogramEntry(gaussian,temp + (current_time - nextServiceFromBuffer.tempo));
                                    nextServiceFromBuffer.tempo = current_time + temp;
                                }
                            }
                        }
                        else if (destino == INEM){
                            nextServiceFromBuffer = hang_up(inem,current_time);
                            if(nextServiceFromBuffer.tempo != -1){
                                civil->active--;
                                nextServiceFromBuffer.tipo = PARTIDA;
                                double temp = nextServiceInterval(inem->lamb,inem->dmin,inem->dmax);
                                nextServiceFromBuffer.tempo = current_time + temp;
                                addHistogramEntry(inem->hist,temp);
                            }
                        }
                        events = adicionar(events,nextServiceFromBuffer);   /* Add PARTIDA event from next event in buffer  */
                    }
                }
                printf("\nEstatisticas de proteçao Civil\n");
                printServerStatus(*civil);

                printf("\nEstatisticas do INEM\n");
                printServerStatus(*inem);
                printf("\nMedia de tempo entre chegada ao sistema e atendimento pelo INEM: %f\n",gaussian->totalCount?(double)gaussian->sum/gaussian->totalCount:0.0);
                resetServer(inem);
                resetServer(civil);
                resetHist(gaussian);
                etotal = 0;
                current_time = 0;
                lista_apaga(events);
                events = NULL;
            }
        }
    }
    saveHistogram("csv/gaussian.csv",gaussian);
    deleteHistogram(gaussian);
   endServer(inem);
   endServer(civil);
   lista_apaga(events);
}