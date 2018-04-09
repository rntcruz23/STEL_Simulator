#include <stdio.h>

typedef struct _event{
    int tipo;
    int destino;
	double tempo;
} event;

typedef struct lista{
    event atual;
	struct lista * proximo;
} lista;

lista * remover (lista * apontador);
lista * adicionar (lista * apontador, event prox);
event nextEvent(lista **list);
event newEvent(int tipo, int destino, double tempo);
void imprimir (lista * apontador);
void imprimirEvent(event e);
void lista_apaga(lista *lst);