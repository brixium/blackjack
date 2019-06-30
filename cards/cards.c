#include "cards.h"

/*Functions*/
void initAndShuffleDeck(){
	initDeck(&deck[0]);
	shuffleDeck(&deck[0]);
	return;
}

void initDeck(card * deck){
	int i;
	cards_in_deck = DECKLEN;
	for(i=0; i<cards_in_deck; i++){
		if(i < CARDS_IN_SUIT){
			(deck+i)->suit = 'C';
			strcpy((deck+i)->suit_l, "hearts\0");
		}else if(i < CARDS_IN_SUIT*2){
			(deck+i)->suit = 'Q';
			strcpy((deck+i)->suit_l, "diamonds\0");
		}else if(i< CARDS_IN_SUIT*3){
			(deck+i)->suit = 'F';
			strcpy((deck+i)->suit_l, "clubs\0");
		}else{
			(deck+i)->suit = 'P';
			strcpy((deck+i)->suit_l, "spades\0");
		}

		if(i%13 == 0){
			(deck+i)->name = 'A';
			strcpy((deck+i)->name_l, "ace");
		}else if(i%13>0 && i%13<9){
			(deck+i)->name = '1'+ (i%13);
			sprintf((deck+i)->name_l, "%d", (i%13)+1);
		}else if(i%13 == 9){
			(deck+i)->name = '0';
			strcpy((deck+i)->name_l, "10\0");
		}else if(i%13 == 10){
			(deck+i)->name = 'J';
			strcpy((deck+i)->name_l, "jack\0");
		}else if(i%13 == 11){
			(deck+i)->name = 'Q';
			strcpy((deck+i)->name_l, "queen\0");
		}else{
			(deck+i)->name = 'K';
			strcpy((deck+i)->name_l, "king\0");
		}
	}
	return;
}

card deal(card * deck){
	int i;
	card c;

	c = *(deck);
	for(i=1; i<cards_in_deck; i++)
		*(deck+i-1) = *(deck+i);
	cards_in_deck--;
	if(cards_in_deck <= 0){
		initDeck(&deck[0]);
		shuffleDeck(&deck[0]);
	}
	return c;
}

void shuffleDeck(card * deck){
	card * ozzam;
	int i, j, r, dim_old_deck;
	time_t t;

	srand(time(&t));
	dim_old_deck = cards_in_deck;
	r = rand()%cards_in_deck;
	if((ozzam = (card *)malloc(sizeof(card)*cards_in_deck))){
		for(i=0; i<cards_in_deck-1; i++){
			*(ozzam+i) =  *(deck+r);
			
			for(j=r; j<dim_old_deck-1; j++)
				*(deck+j) = *(deck+j+1);
			
			dim_old_deck--;
			r = rand()%dim_old_deck;
		}
		*(ozzam+i) = *(deck);
		for(i=0; i<cards_in_deck; i++)
			*(deck+i) = *(ozzam+i);
		free(ozzam);
	}else{
		printf("Not enough RAM\n");
	}
	return;
}

int isDeckOK(card deck[]){
	int flag, conto, i;
	flag = 1;
	for(conto = 0, i=0; i<cards_in_deck; i++)
		conto += deck[i].name + deck[i].suit;
	if(conto != CORRECT_SUM)
		flag = 0;
	return flag;
}

int evaluateCard(card c){
	if(c.name >= '1' && c.name <= '9')
		return (int)(c.name - '0');
	if(c.name == 'J' || c.name == 'Q' || c.name == 'K' || c.name == '0')
		return 10;
	if(c.name == 'A')
		return 11;
	return -1;
}

int aceInDeck(card m[], int dim){
	int i, r;
	for(i=0, r=0; i<dim; i++)
		if(m[i].name == 'A')
			r++;
	return r;
}

void aceCountsAsOne(card * m, int dim){
	int i, flag;
	for(i=0, flag = 0; !flag && i<dim; i++)
		if((m+i)->name == 'A'){
			(m+i)->name = '1';
			flag = 1;
		}
	return;
}

int isBlackJack(card m[], int dim){
	if(dim!=2)
		return 0;
	if(m[0].name == 'A')
		if(m[1].name == 'J' || m[1].name == 'Q' || m[1].name == 'K')
			return 1;
	if(m[0].name == 'J' || m[0].name == 'Q' || m[0].name == 'K')
		if(m[1].name == 'A')
			return 1;
	return 0;
}

int evaluateDeck(card m[], int dim){
	int valore, i;
	for(i=0, valore=0; i<dim; i++)
		valore += evaluateCard(m[i]);
	return valore;
}

void printRawCard(card c){
	printf("%c of %c\n", c.name, c.suit);
	return;
}

void printRawDeck(card m[], int dim){
	int i;
	for(i=0; i<dim; i++)
		printf("%c of %c\n", m[i].name, m[i].suit);
	return;
}
void printCard(card c){
	printf("%s\tof %s\n", c.name_l, c.suit_l);
	return;
}
void printDeck(card m[], int dim){
	int i;
	for(i=0; i<dim; i++)
		printf("%s\tof %s\n", m[i].name_l, m[i].suit_l);
	return;
}

void printDeckHidden(card m[], int dim){
	printf("%s\tof %s\n", m[0].name_l, m[0].suit_l);
	printf("???\tof ???\n");
	return;
}

void print_var_status(){
	printf("\ncards_in_deck: %d\n", cards_in_deck);
	return;
}

