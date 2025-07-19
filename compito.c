#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tipo.h"
#include "liste.h"

//prototipi 
long int int_input(char* msg, int minimum_value, int max_value);
lista load_deck_from_file(char* filename, int cartePerPlayer);
void pesca(lista* deck_user, int v, char s);
void stampa(lista deck_user1, lista deck_user2);
int* tris(lista carte);

int main(){
    //int numero_giocatori = 2;
    int numeroCartePerPlayer = (int) int_input("\nNumero di carta da assegnare ad ogni giocatore: ", 1, (13*4)/2);

    //creazione dei due mazzi
    lista deck_user1 = load_deck_from_file("g1.txt", numeroCartePerPlayer);
    lista deck_user2 = load_deck_from_file("g2.txt", numeroCartePerPlayer);

    stampa(deck_user1, deck_user2);


    //punto 2a
    printf("\n\n--------------\n");
    int dimensione = 13;
    int* array_tris_user1 = tris(deck_user1);
    int* array_tris_user2 = tris(deck_user2);

    printf("\nGiocatore 1: "); 
    for(int i=0; i<dimensione; i++)
        printf(" %d", array_tris_user1[i]);

    printf("\nGiocatore 2: "); 
    for(int i=0; i<dimensione; i++)
        printf(" %d", array_tris_user2[i]);


    printf("\n\n");
    return 0;
}

long int int_input(char* msg, int minimum_value, int max_value){
    long int input;
    char buffer[100];

    printf("%s", msg);
    while(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\nErrore nella lettura dell'input. Riprova: ");
    }

    char *endptr;
    input = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || input < minimum_value || input > max_value) {
        printf("\nInput non valido\n.");
        return int_input(msg, minimum_value, max_value);
    }

    return input;
}

/**
 * @brief Funzione che legge un file testuale (passato come parametro) e crea un mazzo di carta
 * 
 * @param filename 
 * @return lista 
 */
lista load_deck_from_file(char* filename, int cartePerPlayer){
    //printf("\nreading...\n");
    char* mode = "r";

    FILE* fp_deck = fopen(filename, mode);
    if(fp_deck == NULL){printf("\nERRORE"); exit(EXIT_FAILURE);}

    //leggo il file
    char buffer[100];
    lista deck = NULL;
    int cont = 0;

    //ogni riga indica la carta - valore + seme
    while(fgets(buffer, sizeof buffer, fp_deck) != NULL && cont < cartePerPlayer){
        buffer[strcspn(buffer, "\r\n")] = '\0'; 
        //printf("\nRIGA LETTA:%s", buffer);
        int valore;
        char seme; 

        if(sscanf(buffer, "%d %c", &valore, &seme) != 2){printf("\nRIGA NON VALIDA"); fclose(fp_deck); exit(EXIT_FAILURE);}
        if((valore < 1 || valore > 13) || 
           (seme != 'F' && seme != 'Q' && seme != 'P' && seme != 'C') ){

            printf("\nRIGA NON VALIDA");
            fclose(fp_deck);
            exit(EXIT_FAILURE);
        }

        //se i valori sono validi, inserisco la carta nel mazzo
        //printf("\nCARTA DA INSERIRE: %d%c", valore, seme);
        pesca(&deck, valore, seme);
        cont++;
    }

    if(fclose(fp_deck) == EOF){printf("\nERRORE"); exit(EXIT_FAILURE);}
    return deck;

}

/**
 * @brief Funzione che aggiunge una carta ad un mazzo di carte (anche vuoto)
 * 
 * @param deck_user 
 * @param v 
 * @param s 
 */
void pesca(lista* deck_user, int v, char s){
    lista deck_tmp = *deck_user; //creo un deck temporaneo 

    //cre un oggetto temporaneo da aggiungere
    tipo_inf carta = {
        .valore = v, 
        .seme = s
    };

    //inserisco l'oggetto nella lista
    deck_tmp = insert_elem(deck_tmp, new_elem(carta));
    *deck_user = deck_tmp;
}

/**
 * @brief Funzione che stampa la relazione tra le carte dei giocatori
 * 
 * @param deck_user1 
 * @param deck_user2 
 */
void stampa(lista deck_user1, lista deck_user2){
    //scorro entrambe le liste (di egual dimensione)
    printf("\n\nSTAMPA DEI CONFRONTI DEI DUE MAZZI: ");

    while(deck_user1 != NULL && deck_user2 != NULL){
        //procedura per la stampa del messaggio in maniera personalizzata 
        printf("\n\t-");
        print(deck_user1->inf);

        //comparo le carte
        int cmp = compare(deck_user1->inf, deck_user2->inf);
        if(cmp < 0) printf(" e' minore a ");
        else if(cmp > 0) printf(" e' maggiore a ");
        else printf(" e' uguale a ");

        //stampo la carta 2
        print(deck_user2->inf);

        deck_user1 = deck_user1->pun;
        deck_user2 = deck_user2->pun;
    }

}

/**
 * @brief Funzione che restituisce un vettore dinamico in cui v[i] = 1 se fa parte di un tris per valore
 * 
 * @param carte 
 * @return int* 
 */
int* tris(lista carte){
    int dimensione = 13;
    int* contaOccorrenze = calloc(dimensione, sizeof(int));

    if(carte == NULL) return contaOccorrenze;

    //conto quante volte appare quel numero
    while(carte != NULL){
        contaOccorrenze[carte->inf.valore - 1]++;
        carte = carte->pun;
    }

    //se #occorrenze è <3, la divisione intera per 3 dà 0
    for(int i=0; i<dimensione; i++)
        contaOccorrenze[i] = contaOccorrenze[i]/3;

    return contaOccorrenze;
}






