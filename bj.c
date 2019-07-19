#include "cards/cards.h"
#include "money/money.h"
#include "player/player.h"

/*DONE: if the dealer has an ace, ask the player for insurance. If he doesn't want it, ...*/
/*NOT TRIED YET: fix case in which player/dealer has 2 aces (22 pt) and p1 decides to hit*/
/*DONE: fix ace in dealer*/
int main(int argc, char * argv[]){
	/*int nplayer;*/
	
	card player1[MAX_CARDS_4_PLAYER];
	card dealer[MAX_CARDS_4_PLAYER];
	
	/*nplayer = 1;*/
	cli_cycle = 'n'; /*quit?*/
	b_wins = 0;
	p1_wins = 0;
	
	initAndShuffleDeck();
	initMoney();
	/* Ciclo gioco 
	printf("Inserisci il numero di giocatori: ");
	scanf("%d", &nplayer);
	*/
	printf("Di quanto denaro disponi? ");
	scanf("%f", &wallet);
	while((cli_cycle!='y' && cli_cycle!='Y') && wallet > 0){
		/*print_var_status();*/
		gameOver = 0; /* 0=still playing; 1=stand; 2=sballa; 3=max/bj */
		p1_score = 0;
		b_score = 0;
		pp = 0;
		bp = 0;
		round = 0;
		hitorstand = 'O';
		insurance = 'N';

		printf("Insert bet amount: ");
		scanf("%f", &bet);
		wallet = wallet - bet;
		printf("All bets are off!\n");
		while(!gameOver){
			if(round<2){ /*Nei primi 2 giri le carte vengono distribuite e basta*/
				player1[pp] = deal(deck);
				dealer[bp] = deal(deck);
				pp++;
				bp++;
			}else if(round==2){/*Al secondo round ti vengono fatte vedere le carte*/
				/*Stato PLAYER 1*/
				printf("Le tue carte:\n");
				printDeck(player1, pp);
				p1_score = evaluateDeck(player1, pp);
				if(p1_score > 21){
					aceCountsAsOne(player1, pp);
					p1_score = evaluateDeck(player1, pp);
				}
				printf("Hai %d punti nella mano\n", p1_score);
				/*Stato deck*/
				printf("Il dealer:\n");
				printDeckHidden(dealer, bp);
				b_score = evaluateDeck(dealer, bp);
				if(b_score > 21){
					aceCountsAsOne(dealer, bp);
					p1_score = evaluateDeck(dealer, bp);
				}
				/*Se il dealer ha come card scoperta un asso, chiedi al giocatore se vuole fare l'insurance*/
				if(dealer[0].name == 'A'){
					printf("Insurance? [Y/N]");
					scanf(" %c", &insurance);
					/*Add check: if wallet cannot afford insurance, don't let it happen*/
					if(insurance == 'y' ||insurance == 's' || insurance == 'Y' || insurance == 'S'){
					    if(wallet >= bet/2.0)
    						wallet = wallet - (bet/2.0);
    					else
    					    printf("Not enough money for insurance\n");
    				}
				}
			}else{ /*If you're on round 3 or more*/
				/*Scelta: dai o stai*/
				if(b_score == 21 && isBlackJack(dealer, bp))
					gameOver = 1;
				else if(p1_score == 21)
					gameOver = 3;
				else{
					printf("[H]it or [S]tand? [H/S] [D/S]");
					scanf(" %c", &hitorstand);
					if(hitorstand == 'h' || hitorstand == 'd' || hitorstand == 'H' || hitorstand == 'D'){
						player1[pp] = deal(&deck[0]);
						pp++;
						p1_score = evaluateDeck(player1, pp);
						/*Andare avanti a oltranza e valutare sempre se p1 sfora*/
						if(p1_score > 21){
							if(!aceInDeck(player1, pp)){
								/*printf("Le tue carte:\n");
								printDeck(player1, pp);*/
								gameOver = 2;
							}else{
								aceCountsAsOne(&player1[0], pp);
								p1_score = evaluateDeck(player1, pp);
							}
						}else if(p1_score == 21){
							/*printf("Le tue carte:\n");
							printDeck(player1, pp);*/
							gameOver = 1;
						}else{
							/*printf("Le tue carte:\n");
							printfdeck(player1, pp);*/
						}
						/*Ulteriore valutazione del deck*/
						p1_score = evaluateDeck(player1, pp);
						printf("Your cards:\n");
						printDeck(player1, pp);
						printf("Your score: %d\n", p1_score);
					}else if(hitorstand == 'S' || hitorstand == 's'){
						p1_score = evaluateDeck(player1, pp);
						gameOver = 1;
					}
				}
			}
			round++;
		}
		/*È finito il round, si vede il risultato*/
		/*CONDIZIONI DI GAMEOVER*/
		if(gameOver == 2){
			/*p1 sballa, il dealer vince (p1 solo al tavolo)*/
			b_score = evaluateDeck(dealer, bp);
			printf("The dealer:\n");
			printDeck(dealer, bp);
			printf("LOSE! Hai sballato e perso %f euro!\n", bet);
			b_wins++;
		/*Se il player non ha sballato*/
		}else{
			/*Continua il dealer con le sue regole*/
			/*Se il dealer ha blackjack di base vince, il giocatore può fare solamente push solo se ha fatto l'insurance*/
			if(isBlackJack(dealer, bp)){
				printf("The dealer:\n");
				printDeck(dealer, bp);
				if((insurance == 'y' || insurance == 'Y')  && isBlackJack(player1, pp)){
					wallet = wallet + bet;
					printf("PUSH! Non vinci né perdi niente\n");
				}else{
					printf("LOSE! Hai perso! Il dealer ha blackjack\n");
					b_wins++;
				}
			/*Se il dealer non ha fatto bj*/
			}else{
				/*Se il giocatore ha blackjack, viene pagato 3 a 2*/
				if(isBlackJack(player1, pp)){
					printf("WIN! Winner winner chicken dinner! Hai fatto blackjack!\n");
					printf("Il dealer aveva:\n");
					printDeck(dealer, bp);
					wallet += bet*3.0/2;
					p1_wins++;
				}else{
					/*In tutti gli altri casi, il dealer chiede card finché non arriva a 17, dopodiché si ferma*/
					/*Problema che si verifica quando il dealer ha un asso in mano è che non lo considera come 1 quando sballa*/
					if(b_score > 21 && aceInDeck(dealer, bp))
						aceCountsAsOne(&dealer[0], bp);
					while(b_score<17){
						printf("Il dealer:\n");
						printDeck(dealer, bp);

						dealer[bp] = deal(deck);
						bp++;
						b_score = evaluateDeck(dealer, bp);
						if(b_score > 21 && aceInDeck(dealer, bp))
							aceCountsAsOne(&dealer[0], bp);
						b_score = evaluateDeck(dealer, bp);
					}
					/*Confronto del risultato con quello del giocatore 1*/
					printf("Il dealer:\n");
					printDeck(dealer, bp);
					b_score = evaluateDeck(dealer, bp);/*Dovrebbe essere inutile ma last resort*/
					if(b_score > 21){
						wallet += 2*bet;
						printf("WIN! Il dealer sballa, bella per voi!\n");
						p1_wins++;
					}else if(b_score > p1_score){
						printf("LOSE! Peccato! Il dealer ha totalizzato %d punti, contro i tuoi %d. Ciò significa che hai perso.\n", b_score, p1_score);
						b_wins++;
					}else if(b_score < p1_score){
						wallet += 2*bet;
						printf("WIN! Bella per P1 che totalizza %d punti e il dealer solo %d, vincendo i soldi!\n", p1_score, b_score);
						p1_wins++;
					}else if(b_score == p1_score){
						wallet += bet;
						printf("PUSH! Non hai vinto né perso in quanto sia tu che il dealer avete totalizzato %d punti!\n", b_score);
					}
				}
			}
		}
		printf("Il tuo bilancio è di %f euro\n", wallet);
		printf("WOULD YOU LIKE TO QUIT? [Y/N] (default=no)");
		scanf(" %c", &cli_cycle);
	}
	printf("You won %d times\nThe dealer won %d times\n", p1_wins, b_wins);
	return 0;
}

