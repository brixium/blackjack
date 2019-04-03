#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DIMMAZZO 52
#define CARTE_PER_SEME 13
#define MAX_LEN_NOME 5
#define MAX_LEN_SUIT 8
#define CONTO_CORRETTO 6789

#define MAX_CARTE_X_PLAYER 11

typedef struct carta_s{
	char nome; /*A,2,3,4,5,6,7,8,9,0,J,Q,K*/
	char suit; /*C,Q,F,P*/
	char nome_l[MAX_LEN_NOME+1]; /*Ace, 1, 2, 3, ..., 10, Jack, Queen, King*/
	char suit_l[MAX_LEN_SUIT+1]; /*hearts(C), diamonds(Q), clubs(F) and spades(P)*/
}carta;

/*Variabili per il gioco*/
int portafoglio;
int puntata;
int side_bet_1;
int side_bet_2;
int puntata_min;
int n_round;
int carte_nel_mazzo;

/*Funzioni di libreria per il blackjack*/
void init_game();					/*Inizializza le variabili di gioco*/
void init_mazzo(carta[]);	 		/*Inizializza il mazzo di carte francese da 52 carte*/
carta deal(carta[]);				/*Prende una carta dal mazzo, eliminandola da lì, e la restituisce*/
void printRawCarta(carta);			/*Stampa carta con valori contenuti nella struct carta_s*/
void printRawMazzo(carta[]);		/*Stamapa mazzo con valori delle variabili*/
void printCarta(carta);				/*Stampa carta con valori human friendly*/
void printMazzo(carta[], int);		/*Stampa mazzo con valori umani, ingresso anche dimensione*/
void mischiaMazzo(carta *);			/*Mescola il mazzo ordinando le carte in modo casuale*/
int isMazzoOK(carta[]);				/*Controlla che un mazzo sia composto da tutte e sole le carte necessarie e sufficienti*/
int evaluateMazzo(carta[], int);	/*Restituisce il valore del mazzo*/
int evaluateCarta(carta);			/*Restituisce il valore della carta (asso vale 11 di default)*/
int assoInMazzo(carta[], int);		/*Restituisce il numero di assi nel mazzo*/
void assoValeUno(carta *, int);
/*Le funzioni*/
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

int isMazzoOK(carta mazzo[]){
	int flag, conto, i;

	flag = 1;
	conto = 0;
	for(i=0; i<carte_nel_mazzo; i++){
		conto += mazzo[i].nome;
		conto += mazzo[i].suit;
	}
	if(conto != CONTO_CORRETTO)
		flag = 0;
	return flag;
}

int evaluateCarta(carta c){
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
	return r;
}

void assoValeUno(carta * m, int dim){
	int i, flag;
	carta * nm = NULL;
	if((nm = (carta *)malloc(sizeof(carta)*dim))){
		for(i=0, flag =0;!flag && i<dim; i++){
			if((nm+i)->nome == 'A')
				(nm+i)->nome = '1';
			else
				*(nm+i) = *(m);
		}
		m = nm;
		free(nm);
	}else
		printf("No memory\n");
}

int evaluateMazzo(carta m[], int dim){
	int valore, i;
	valore = 0;
	for(i=0; i<dim; i++)
		valore += evaluateCarta(m[i]);
	return valore;
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
	printf("%s\tof %s\n", c.nome_l, c.suit_l);
	return;
}
void printMazzo(carta m[], int dim){
	int i;
	for(i=0; i<dim; i++){
		printf("%s\tof %s\n", m[i].nome_l, m[i].suit_l);
	}
	return;
}

void print_var_status(){
	printf("\nPortafoglio: %d\nPuntata: %d\nPuntata_min: %d\nN_round: %d\nCarte_nel_mazzo: %d\n", portafoglio, puntata, puntata_min, n_round, carte_nel_mazzo);
	return;
}
