#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DIMMAZZO 52
#define CARTE_PER_SEME 13
#define MAX_LEN_NOME 5
#define MAX_LEN_SUIT 8
#define CONTO_CORRETTO 6789

#define MAX_CARTE_X_PLAYER 11

typedef struct carta_s{
	char nome; /*1,2,3,4,5,6,7,8,9,0,J,Q,K,A*/
	char suit; /*C,Q,F,P*/
	char nome_l[MAX_LEN_NOME+1]; /*Ace, 1, 2, 3, ..., 10, Jack, Queen, King*/
	char suit_l[MAX_LEN_SUIT+1]; /*hearts(C), diamonds(Q), clubs(F) and spades(P)*/
}carta;

/*Variabili per il gioco*/
float portafoglio;
float puntata;
int side_bet_1;
int side_bet_2;
int puntata_min;
/*int n_round;*/
int carte_nel_mazzo;
carta mazzo[DIMMAZZO];

/*Funzioni di libreria per il blackjack*/
void init_game();						/*Inizializza le variabili di gioco*/
void init_mazzo(carta *);	 			/*Inizializza il mazzo di carte francese da 52 carte*/
carta deal(carta *);					/*Prende una carta dal mazzo, eliminandola da lì, e la restituisce*/
void printRawCarta(carta);				/*Stampa carta con valori contenuti nella struct carta_s*/
void printRawMazzo(carta[], int);		/*Stamapa mazzo con valori delle variabili*/
void printCarta(carta);					/*Stampa carta con valori human friendly*/
void printMazzo(carta[], int);			/*Stampa mazzo con valori umani, ingresso anche dimensione*/
void mischiaMazzo(carta *);				/*Mescola il mazzo ordinando le carte in modo casuale*/
int isMazzoOK(carta[]);					/*Controlla che un mazzo sia composto da tutte e sole le carte necessarie e sufficienti*/
int evaluateMazzo(carta[], int);		/*Restituisce il valore del mazzo*/
int evaluateCarta(carta);				/*Restituisce il valore della carta (asso vale 11 di default)*/
int assoInMazzo(carta[], int);			/*Restituisce il numero di assi nel mazzo*/
void assoValeUno(carta *, int);			/*Modifica il primo asso del mazzo in ingresso, dandogli valore 1 anziché 11*/
int isBlackJack(carta[], int);			/*Controlla se il mazzo inserito è blackjack*/
void printMazzoNascosto(carta[], int);	/*Stampa il mazzo con la seconda carta nascosta*/

/*Le funzioni*/
void init_game(){
	portafoglio = 0;
	puntata = 0;
	puntata_min = 1;

	init_mazzo(&mazzo[0]);
	mischiaMazzo(&mazzo[0]);
	return;
}

void init_mazzo(carta * mazzo){
	int i;
	carte_nel_mazzo = DIMMAZZO;
	for(i=0; i<carte_nel_mazzo; i++){
		if(i < CARTE_PER_SEME){
			(mazzo+i)->suit = 'C';
			strcpy((mazzo+i)->suit_l, "hearts\0");
		}else if(i < CARTE_PER_SEME*2){
			(mazzo+i)->suit = 'Q';
			strcpy((mazzo+i)->suit_l, "diamonds\0");
		}else if(i< CARTE_PER_SEME*3){
			(mazzo+i)->suit = 'F';
			strcpy((mazzo+i)->suit_l, "clubs\0");
		}else{
			(mazzo+i)->suit = 'P';
			strcpy((mazzo+i)->suit_l, "spades\0");
		}

		if(i%13 == 0){
			(mazzo+i)->nome = 'A';
			strcpy((mazzo+i)->nome_l, "ace");
		}else if(i%13>0 && i%13<9){
			(mazzo+i)->nome = '1'+ (i%13);
			sprintf((mazzo+i)->nome_l, "%d", (i%13)+1);
		}else if(i%13 == 9){
			(mazzo+i)->nome = '0';
			strcpy((mazzo+i)->nome_l, "10\0");
		}else if(i%13 == 10){
			(mazzo+i)->nome = 'J';
			strcpy((mazzo+i)->nome_l, "jack\0");
		}else if(i%13 == 11){
			(mazzo+i)->nome = 'Q';
			strcpy((mazzo+i)->nome_l, "queen\0");
		}else{
			(mazzo+i)->nome = 'K';
			strcpy((mazzo+i)->nome_l, "king\0");
		}
	}
	return;
}

carta deal(carta * mazzo){
	int i;
	carta c;

	c = *(mazzo);
	for(i=1; i<carte_nel_mazzo; i++)
		*(mazzo+i-1) = *(mazzo+i);
	carte_nel_mazzo--;
	if(carte_nel_mazzo <= 0){
		init_mazzo(&mazzo[0]);
		mischiaMazzo(&mazzo[0]);
	}
	return c;
}

void mischiaMazzo(carta * mazzo){
	carta * ozzam;
	int i, j, r, dim_old_mazzo;
	time_t t;

	srand(time(&t));
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

int isMazzoOK(carta mazzo[]){
	int flag, conto, i;
	flag = 1;
	for(conto = 0, i=0; i<carte_nel_mazzo; i++)
		conto += mazzo[i].nome + mazzo[i].suit;
	if(conto != CONTO_CORRETTO)
		flag = 0;
	return flag;
}

int evaluateCarta(carta c){
	/*codice di debug
	if(c.nome == '1'){
		int x = (int)(c.nome - '0');
		printf("Questo asso vale %d\n", x);
	}
	Fine debug*/
	if(c.nome >= '1' && c.nome <= '9')
		return (int)(c.nome - '0');
	if(c.nome == 'J' || c.nome == 'Q' || c.nome == 'K' || c.nome == '0')
		return 10;
	if(c.nome == 'A')
		return 11;
	return -1;
}

int assoInMazzo(carta m[], int dim){
	int i, r;
	for(i=0, r=0; i<dim; i++)
		if(m[i].nome == 'A')
			r++;
	/*RIGA DI DEBUG
	printf("Ci sono %d assi nel mazzo\n", r);
	*/
	return r;
}

void assoValeUno(carta * m, int dim){
	int i, flag;
	for(i=0, flag = 0; !flag && i<dim; i++)
		if((m+i)->nome == 'A'){
			(m+i)->nome = '1';
			/*DEBUG PRINT*/
			printf("Ho modificato l'asso in posizione %d\n", i);
		}
	return;
}

int isBlackJack(carta m[], int dim){
	if(dim!=2)
		return 0;
	if(m[0].nome == 'A')
		if(m[1].nome == 'J' || m[1].nome == 'Q' || m[1].nome == 'K')
			return 1;
	if(m[0].nome == 'J' || m[0].nome == 'Q' || m[0].nome == 'K')
		if(m[1].nome == 'A')
			return 1;
	return 0;
}

int evaluateMazzo(carta m[], int dim){
	int valore, i;
	for(i=0, valore=0; i<dim; i++)
		valore += evaluateCarta(m[i]);
	return valore;
}

void printRawCarta(carta c){
	printf("%c of %c\n", c.nome, c.suit);
	return;
}

void printRawMazzo(carta m[], int dim){
	int i;
	for(i=0; i<dim; i++)
		printf("%c of %c\n", m[i].nome, m[i].suit);
	return;
}
void printCarta(carta c){
	printf("%s\tof %s\n", c.nome_l, c.suit_l);
	return;
}
void printMazzo(carta m[], int dim){
	int i;
	for(i=0; i<dim; i++)
		printf("%s\tof %s\n", m[i].nome_l, m[i].suit_l);
	return;
}

void printMazzoNascosto(carta m[], int dim){
	printf("%s\tof %s\n", m[0].nome_l, m[0].suit_l);
	printf("???\tof ???\n");
	return;
}

void print_var_status(){
	printf("\nPortafoglio: %f\nPuntata: %f\nPuntata_min: %d\nCarte_nel_mazzo: %d\n", portafoglio, puntata, puntata_min, carte_nel_mazzo);
	return;
}
