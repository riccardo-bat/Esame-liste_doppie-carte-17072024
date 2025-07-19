#include "tipo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Funzione per la comparazione di due carte in base al valore e al seme
 * 
 * @param firstCard 
 * @param secondCard 
 * @return int 
 */
int compare(tipo_inf firstCard, tipo_inf secondCard){
    if(firstCard.valore < secondCard.valore) return -1;
    else if(firstCard.valore > secondCard.valore) return 1;

    //se hanno lo stesso valore, ...
    if(firstCard.seme < secondCard.seme) return -1;
    else if(firstCard.seme > secondCard.seme) return 1;

    return 0;
}

/**
 * @brief Funzione di copia di una carta
 * 
 * @param dest 
 * @param src 
 */
void copy(tipo_inf* dest, tipo_inf src){
    *dest = src;
}

/**
 * @brief Funzione di stampa di una carta
 * 
 * @param carta 
 */
void print(tipo_inf carta){
    printf("%d%c", carta.valore, carta.seme);
}