#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tipo.h"
#include "liste.h"

//prototipi 
long int int_input(char* msg, int minimum_value, int max_value);
char* string_input(char* msg, int max_length);
lista load_deck_from_file(char* filename, int cartePerPlayer);
void pesca(lista* deck_user, int v, char s);
void stampa(lista deck_user1, lista deck_user2);
int* tris(lista carte);
int cala(lista* carte);

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


    //punto 2b
    printf("\n\n--------------\n");
    //ogni giocatore pesca una carta
    printf("\n\nPESCAGGIO GIOCATORE 1...");  
    pesca(&deck_user1, (int) int_input("\nValore della carta: ", 1, 13), string_input("\nSeme della carta: ", 2)[0]); 
    printf("\n\nPESCAGGIO GIOCATORE 2..."); 
    pesca(&deck_user2, (int) int_input("\nValore della carta: ", 1, 13), string_input("\nSeme della carta: ", 1)[0]); 

    //punteggi totali
    printf("\nGiocatore 1 cala: ");
    int punteggio_user1 = 0;
    int res;
    do{
        res = cala(&deck_user1);
        punteggio_user1 += res;
    }while(res != 0);
    printf(". Punteggio totale: %d", punteggio_user1);

    printf("\nGiocatore 2 cala: ");
    int punteggio_user2 = 0;
    do{
        res = cala(&deck_user2);
        punteggio_user2 += res;
    }while(res != 0);
    printf(". Punteggio totale: %d", punteggio_user2);

    //verifico se uno dei due giocatori ha terminato le carte, ed in caso stampando il vincitore
    if(deck_user1 == NULL || deck_user2 == NULL){
        if(punteggio_user1 > punteggio_user2) printf("\n\nIL GIOCATORE 1 HA VINTO");
        else if(punteggio_user2 > punteggio_user1) printf("\n\nIL GIOCATORE 1 HA VINTO");
        //else --> non stampo nulla, non ha vinto nessuno
    }

    //else --> non ha vinto nessuno




    //libero la memoria allocata
    free(array_tris_user1);
    free(array_tris_user2);

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

char* string_input(char* msg, int max_length){
    printf("%s", msg); 
    char* buffer = malloc(max_length * sizeof(char));
    if(!buffer){printf("\n\nMalloc failed"); exit(EXIT_FAILURE);} 

    while(fgets(buffer, (max_length+1), stdin) == NULL){
        printf("\nInput fallito\n");
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    //svuoto il buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    return buffer;
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
 * @brief Funzione che restituisce un vettore dinamico in cui v[i] = 1 se fa parte di un tris per valore. la dimensione è pari a 13
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


/**
 * @brief Funzione che cala un tris di carte presenti in un mazzo, restituendo il punteggio finale ottenuto dal giocatore dopo aaver calatato tutti i tris
 * 
 */
int cala(lista* carte){
    lista tmp_deck = *carte; 
    int punteggioTotale = 0;

    int* trisDisponibili = tris(tmp_deck);
    //verifico se è presente almeno un tris
    int carteDaButtare = 0;
    int daCalare = 0;

    for(int i=0; i<13 && carteDaButtare == 0; i++){
        if(trisDisponibili[i] != 0){
            carteDaButtare = trisDisponibili[i]*3; 
            daCalare = i+1; //valore da calare
        }
    }

    if(carteDaButtare == 0) return punteggioTotale;

    //se è presente almeno un tris di un determinato valore, lo calo
    //scorro la lista per calare un tris di daCalare
    lista cursor = tmp_deck;
    while(cursor != NULL && carteDaButtare > 0){
        if(head(cursor).valore == daCalare){ //se posso calare una carta appartenente ad un tris...
            print(head(cursor));
            printf(" ");
            carteDaButtare--; //scalo il #carte da buttare
            lista to_delete = cursor; //creo un nodo per non eliminare direttamente cursor
            tmp_deck = delete_elem(tmp_deck, to_delete); //elimino la carta dal mazzo
            punteggioTotale += daCalare; //incremento il punteggio totale del giocatore
        }

        cursor = tail(cursor);
    }
    

    *carte = tmp_deck;
    return punteggioTotale;
}


