#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DECKLEN 52
#define CARDS_IN_SUIT 13
#define NAME_LEN_MAX_EN 5
#define NAME_LEN_SUIT_EN 8
#define CORRECT_SUM 6789

#define MAX_CARDS_4_PLAYER 11

typedef struct card_s{
	char name; /*1,2,3,4,5,6,7,8,9,0,J,Q,K,A*/
	char suit; /*C,Q,F,P*/
	char name_l[NAME_LEN_MAX_EN+1]; /*Ace, 1, 2, 3, ..., 10, Jack, Queen, King*/
	char suit_l[NAME_LEN_SUIT_EN+1]; /*hearts(C), diamonds(Q), clubs(F) and spades(P)*/
}card;

/*Variabili per le carte*/
/*int n_round;*/
int cards_in_deck;
card deck[DECKLEN];

/*Funzioni di libreria per il blackjack*/
void initAndShuffleDeck();			/*Inizializza le variabili di gioco*/
void initDeck(card *);	 			/*Inizializza il deck di carte francese da 52 carte*/
card deal(card *);					/*Prende una card dal deck, eliminandola da lì, e la restituisce*/
void printRawCard(card);			/*Stampa card con valori contenuti nella struct card_s*/
void printRawDeck(card[], int);		/*Stamapa deck con valori delle variabili*/
void printCard(card);				/*Stampa card con valori human friendly*/
void printDeck(card[], int);		/*Stampa deck con valori umani, ingresso anche dimensione*/
void shuffleDeck(card *);			/*Mescola il deck ordinando le carte in modo casuale*/
int isDeckOK(card[]);				/*Controlla che un deck sia composto da tutte e sole le carte necessarie e sufficienti*/
int evaluateDeck(card[], int);	    /*Restituisce il valore del deck*/
int evaluateCard(card);				/*Restituisce il valore della card (asso vale 11 di default)*/
int aceInDeck(card[], int);			/*Restituisce il numero di assi nel deck*/
void aceCountsAsOne(card *, int);	/*Modifica il primo asso del deck in ingresso, dandogli valore 1 anziché 11*/
int isBlackJack(card[], int);		/*Controlla se il deck inserito è blackjack*/
void printDeckHidden(card[], int);	/*Stampa il deck con la seconda card nascosta*/

