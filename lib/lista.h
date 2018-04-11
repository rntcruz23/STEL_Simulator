#include <stdio.h>

typedef struct _event{
    int tipo;               /*  Type of event {CHEGADA|REDIRECT|PARTIDA}    */
    int destino;            /*  Destination of event {CIVIL|INEM}           */
	double tempo;           /*  Time the event occurs                       */
} event;

typedef struct lista{
    event atual;
	struct lista * proximo; /*  Pointer to next event                       */
} lista;
/**
 * Removes next event from the list
 * @param apontador         -   Pointer to the event list
 * @return                  -   Return list
 * */
lista * remover (lista * apontador);
/**
 * Add new event to list, keeps list time order
 * Looks for events with time == -1, does not add them
 * @param apontador         -   Pointer to event list
 * @param prox              -   Event to add
 * @return                  -   Return list with new event
 * */
lista * adicionar (lista * apontador, event prox);
/**
 * Removes next event from list and returns that event
 * @param list              -   Pointer of pointer to list
 * @return                  -   Next event from list
 * */
event nextEvent(lista **list);
/**
 * Creates new event var
 * @param tipo              -   Type of event {CHEGADA|REDIRECT|PARTIDA}
 * @param destino           -   Destination of event {CIVIL|INEM}  
 * @param tempo             -   Time the event occurs
 * @return                  -   New event
 * */
event newEvent(int tipo, int destino, double tempo);
/**
 * Prints event list
 * @param apontador         -   Pointer to list   
 **/ 
void imprimir (lista * apontador);
/**
 * Print event information
 * @param e                  -  Event to print    
 * */
void imprimirEvent(event e);
/**
 * Delete list, freeing all memory allocated
 * @param lst                 -  Pointer to list
 * */
void lista_apaga(lista *lst);