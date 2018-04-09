#include <stdlib.h>
#include "lista.h"

// Fun��o que remove o primeiro elemento da lista
lista * remover (lista * apontador)
{
	if(!apontador) return NULL;
	lista * lap = (lista *)apontador->proximo;
	free(apontador);
	return lap;
}
event nextEvent(lista **list){
    event prox = (*list)->atual;
    *list =  remover(*list);
    return prox;
}
// Fun��o que adiciona novo elemento � lista, ordenando a mesma por tempo
lista * adicionar (lista * apontador, event prox)
{
	if(prox.tempo == -1) return apontador;

    double n_tempo = prox.tempo;
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador->proximo = NULL;
		apontador->atual = prox;
		return apontador;
	}
	else
	{
		if (apontador-> atual.tempo > n_tempo) {
	        ap_aux = (lista *) malloc(sizeof (lista));
			ap_aux->atual = prox;
            ap_aux->proximo = (struct lista *) apontador;
            return ap_aux;
	    }

		ap_next = (lista *)apontador->proximo;
		while(apontador != NULL)
		{
			if((ap_next == NULL) || ((ap_next->atual . tempo) > n_tempo))
				break;
			apontador = (lista *)apontador->proximo;
			ap_next = (lista *)apontador->proximo;
		}
		ap_aux = (lista *)apontador->proximo;
		apontador->proximo = (struct lista *) malloc(sizeof (lista));
		apontador = (lista *)apontador->proximo;
		if(ap_aux != NULL)
			apontador->proximo = (struct lista *)ap_aux;
		else
			apontador->proximo = NULL;
		apontador->atual = prox;
		return lap;
	}
}
// Fun��o que imprime no ecra todos os elementos da lista
void imprimirEvent(event e){
	printf("Tipo=%d\tDestino=%d\tTempo=%f\n",e.tipo,e.destino,e.tempo);
}
void imprimir (lista * apontador)
{
	if(apontador == NULL)
		printf("Lista vazia!\n");
	else
	{
		while(apontador != NULL)
		{
			imprimirEvent(apontador->atual);
			apontador = (lista *)apontador->proximo;
		}
	}
}
void lista_apaga(lista *lst){
    lista *aux1;
    lista *aux2;
    aux1 = aux2 = lst;
    while(aux1){
        aux1 = aux2->proximo;
        free(aux2);
        aux2 = aux1;
    }
}
event newEvent(int tipo, int destino, double tempo){
    event new;
    new.tipo = tipo;
    new.destino = destino;
    new.tempo = tempo;

    return new;
}