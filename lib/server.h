#include "lista.h"
#include "distributions.h"
#include "predictions.h"

#define FREE    1
#define TOBUFF  2
#define BLOCKED 3

#define CHEGADA         0
#define PARTIDA         1
#define REDIRECT        2

#define CIVIL           0
#define INEM            1

#define PRINT(X) {if (X == CIVIL)  printf("CIVIL"); else printf("INEM");}

typedef struct _server{
    int active;                 /* Busy Channels                                            */
    int delays;                 /* Buffer Ocuppation                                        */

    int blocks;                 /* Blocked counter                                          */
    int delayed;                /* Delayed users count                                      */
    int totalReceived;          /* Total calls arrived                                      */
    double delayTime;

    int channels;               /* Total channels                                           */
    int bufferSize;             /* Queue size                                               */

    int arrivalType;            /* Effective arrival type: INEM->REDIRECT; CIVIL->CHEGADA   */
    int serviceType;            /* Server type: INEM; CIVIL */
    double lamb;                /* Service rate                                             */
    int dmin,dmax;              /* Service min,max                                          */ 

    prediction *predict;        /* Queue time prediction model                              */
    histogram *hist;            /* Service histogram                                        */
    lista *buffer;              /* Queue buffer                                             */
} server;

server *newServer(int m,int maxD,int arrival,int serviceType,double lamb,int dmin,int dmax);
void endServer(server *s);
int pick_up(server *s,event e);
event hang_up(server *s,double currentTime);
event addEvent(server *s,event new);
void printServerStatus(server s);
void resetServer(server *s);