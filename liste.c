#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"


// Restituisce il valore dell'elemento
tipo_inf head(lista p) {
    return p->inf;
}

// Restituisce il successivo
lista tail(lista p) {
    return p->pun;
}

// Restituisce il precedente
lista prev(lista p) {
    return p->prev;
}

// Cerca il primo elemento che contiene valore v, ritorna NULL se non trovato
lista search(lista l, tipo_inf v) {
    while (l != NULL) {
        if (compare(head(l), v) == 0)
            return l;
        l = tail(l);
    }
    return NULL;
}

// Crea un nuovo elemento con valore inf
struct elem* new_elem(tipo_inf inf) {
    struct elem* p = (struct elem*)malloc(sizeof(struct elem));
    if (p == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    copy(&(p->inf), inf);
    p->pun = NULL;
    p->prev = NULL;
    return p;
}

// Inserisce l'elemento e in testa alla lista l e restituisce la nuova testa
lista insert_elem(lista l, struct elem* e) {
    e->pun = l;
    if (l != NULL)
        l->prev = e;
    e->prev = NULL;
    return e;
}

// Elimina l'elemento e dalla lista l e restituisce la nuova testa
lista delete_elem(lista l, struct elem* e) {
    if (l == e) {
        l = e->pun;  // e è testa
    } else {
        e->prev->pun = e->pun;
    }
    if (e->pun != NULL)
        e->pun->prev = e->prev;
    free(e);
    return l;
}
