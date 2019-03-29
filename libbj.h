#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DIMMAZZO 52
#define CARTE_PER_SEME 13
#define MAX_LEN_NOME 5
#define MAX_LEN_SUIT 8

typedef struct carta_s{
	char nome; /*A,2,3,4,5,6,7,8,9,0,J,Q,K*/
	char suit; /*C,Q,F,P*/
	char nome_l[MAX_LEN_NOME+1]; /*Ace, 1, 2, 3, ..., 10, Jack, Queen, King*/
	char suit_l[MAX_LEN_SUIT+1]; /*hearts(C), diamonds(Q), clubs(F) and spades(P)*/
}carta;

/*Variabili per il gioco*/
int portafoglio;
int puntata;
int puntata_min;
int n_round;
int carte_nel_mazzo;

/*Metodi di libreria per il blackjack*/
void init_game();					/*Inizializza le variabili di gioco*/
void init_mazzo(carta[]);	 		/*Inizializza il mazzo di carte francese da 52 carte*/
carta deal(carta[]);				/*Prende una carta dal mazzo, eliminandola da lì, e la restituisce*/
void printRawCarta(carta);			/*Stampa carta con valori contenuti nella struct carta_s*/
void printRawMazzo(carta[]);		/*Stamapa mazzo con valori delle variabili*/
void printCarta(carta);				/*Stampa carta con valori human friendly*/
void printMazzo(carta[]);			/*Stampa mazzo con valori umani*/
void mischiaMazzo(carta *);			/*Mescola il mazzo ordinando le carte in modo casuale*/

void init_game(){
	carte_nel_mazzo = DIMMAZZO;
	portafoglio = 0;
	puntata = 0;
	puntata_min = 1;
	n_round = -1;

	return;
}

void init_mazzo(carta mazzo[]){
	int i;
	for(i=0; i<carte_nel_mazzo; i++){
		if(i < CARTE_PER_SEME){
			mazzo[i].suit = 'C';
			strcpy(mazzo[i].suit_l, "hearts\0");
		}else if(i < CARTE_PER_SEME*2){
			mazzo[i].suit = 'Q';
			strcpy(mazzo[i].suit_l, "diamonds\0");
		}else if(i< CARTE_PER_SEME*3){
			mazzo[i].suit = 'F';
			strcpy(mazzo[i].suit_l, "clubs\0");
		}else{
			mazzo[i].suit = 'C';
			strcpy(mazzo[i].suit_l, "spades\0");
		}

		if(i%13 == 0){
			mazzo[i].nome = 'A';
			strcpy(mazzo[i].nome_l, "ace");
		}else if(i%13>0 && i%13<9){
			mazzo[i].nome = '1'+ (i%13);
			sprintf(mazzo[i].nome_l, "%d", (i%13)+1);
		}else if(i%13 == 9){
			mazzo[i].nome = '0';
			strcpy(mazzo[i].nome_l, "10\0");
		}else if(i%13 == 10){
			mazzo[i].nome = 'J';
			strcpy(mazzo[i].nome_l, "jack\0");
		}else if(i%13 == 11){
			mazzo[i].nome = 'Q';
			strcpy(mazzo[i].nome_l, "queen\0");
		}else{
			mazzo[i].nome = 'K';
			strcpy(mazzo[i].nome_l, "king\0");
		}
	}
	return;
}

carta deal(carta mazzo[]){
	int i;
	carta c = mazzo[0];
	for(i=1; i<carte_nel_mazzo; i++)
		mazzo[i-1] = mazzo[i];
	carte_nel_mazzo--;
	return c;
}

void mischiaMazzo(carta * mazzo){
	carta * ozzam;
	int i, j, r, dim_old_mazzo;

	dim_old_mazzo = carte_nel_mazzo;
	r = rand()%carte_nel_mazzo;
	if((ozzam = (carta *)malloc(sizeof(carta)*carte_nel_mazzo))){
		for(i=0; i<carte_nel_mazzo-1; i++){
			*(ozzam+i) =  *(mazzo+r);
			
			for(j=r; j<dim_old_mazzo-1; j++)
				*(mazzo+j) = *(mazzo+j+1);
			
			dim_old_mazzo--;
			r = rand()%dim_old_mazzo;
		}
		*(ozzam+i) = *(mazzo);
		for(i=0; i<carte_nel_mazzo; i++)
			*(mazzo+i) = *(ozzam+i);
		free(ozzam);
	}else{
		printf("Libera sta RAM\n");
	}
	return;
}

void printRawCarta(carta c){
	printf("%c of %c\n", c.nome, c.suit);
	return;
}

void printRawMazzo(carta m[]){
	int i;
	for(i=0; i<carte_nel_mazzo; i++)
		printf("%c of %c\n", m[i].nome, m[i].suit);
	return;
}
void printCarta(carta c){
	printf("%s of %s\n", c.nome_l, c.suit_l);
	return;
}
void printMazzo(carta m[]){
	int i;
	for(i=0; i<carte_nel_mazzo; i++){
		printf("%s of %s\n", m[i].nome_l, m[i].suit_l);
	}
	return;
}

void print_var_status(){
	printf("\nPortafoglio: %d\nPuntata: %d\nPuntata_min: %d\nN_round: %d\nCarte_nel_mazzo: %d\n", portafoglio, puntata, puntata_min, n_round, carte_nel_mazzo);
	return;
}
