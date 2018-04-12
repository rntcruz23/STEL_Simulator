#include "server.h"
#include <stdlib.h>
#include <stdio.h>

server *newServer(int m,int bufferSize,int arrival,int serviceType,double lamb,int dmin,int dmax){
    server *new = (server *)calloc(1,sizeof(server));

    new->hist = newHistogram(10000);
    new->predict = newPredictModel();
    new->bufferSize = bufferSize;
    new->channels = m;
    new->arrivalType = arrival;
    new->serviceType = serviceType;
    new->lamb = lamb;
    new->dmin = dmin;
    new->dmax = dmax;

    return new;
}
void endServer(server *s){
    char title[100];
    sprintf(title,"%s/service%d.%s",OUTPUT,s->serviceType,OUTPUT);
    saveHistogram(title,s->hist);
    sprintf(title,"%s/predict%d.%s",OUTPUT,s->serviceType,OUTPUT);
    saveHistogram(title,s->predict->h);

    deleteHistogram(s->hist);
    deletePredictions(s->predict);
    lista_apaga(s->buffer);
    free(s);
}

int pick_up(server *s,event e){
    s->totalReceived++;
    if(s->active < s->channels){
        s->active++;
        return FREE;
    }
    if(s->delays < s->bufferSize){
        s->delays++;
        s->delayed++;
        addEvent(s,e);
        double prediction =  getCurrentPrediction(s->predict);
        /*
        printf("Current prediction for ");*/
        /*PRINT(s->serviceType);
        printf(" :%f\n",prediction*s->delays);*/
    
        addPredictionEntry(s->predict,s->delays?prediction/s->delays:0);
        
        return TOBUFF;
    }
    s->blocks++;
    return BLOCKED;
}
event hang_up(server *s,double currentTime){
    event nextService;
    nextService.tempo = -1;
    s->active--;
    if(s->delays > 0){
        s->delays--;
        s->active++;

        nextService = nextEvent(&s->buffer);
        s->delayTime += currentTime - nextService.tempo;
        addCorrectEntry(s->predict,s->delays?(currentTime - nextService.tempo)/s->delays:0);     
    }
    return nextService;
}
event addEvent(server *s,event new){
    s->buffer = adicionar(s->buffer,new);
    return new;
}

void printServerStatus(server s){
    printf("Total serviced users: %d from %d received calls\n",s.hist->totalCount,s.totalReceived);
    printf("Probabilidade de delay: %f\n",s.totalReceived?(double)s.delayed/s.totalReceived:0.0);
    printf("Media de tempo de espera em fila: %f\n",s.delayed?(double)s.delayTime/s.delayed:0.0);
    printf("Probabilidade de block: %f\n",s.totalReceived?(double)s.blocks/s.totalReceived:0.0);
    printf("Service time average: %f\n",s.hist->totalCount?(double)s.hist->sum/s.hist->totalCount:0.0);
}

void resetServer(server *s){
    resetHist(s->hist);
    s->active = 0;
    s->delayed = 0;
    s->delays = 0;
    s->totalReceived = 0;
    s->blocks = 0;
    s->delayTime = 0;
}