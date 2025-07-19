/*******************************/
/* HEADER MODULO "liste" */
/*******************************/

#ifndef LISTE_H
#define LISTE_H

#include "tipo.h"
struct elem
{
       tipo_inf inf;
       struct elem* pun ;
       struct elem* prev;
} ;

typedef struct elem* lista ;
	
lista tail(lista);
lista prev(lista);
lista insert_elem(lista, struct elem*);
lista delete_elem(lista, struct elem*);

tipo_inf head(lista);
struct elem* new_elem(tipo_inf);
struct elem* search(lista, tipo_inf);

#endif
