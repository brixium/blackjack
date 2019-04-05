#include "libbj.h"

/*DONE: se il banco ha asso scoperto, chiedere al giocatore l'assicurazione. Se non la fa, ...*/
/*NOT TRIED YET: fix case in wich player/banco has 2 aces (22 pt) and p1 decides to hit*/
/*DONE: fix ace in banco*/
int main(int argc, char * argv[]){
	char flag, hitorstand, assicurazione;
	/*int nplayer;*/
	int giro, pp, bp, gameOver, p1_score, b_score, b_wins, p1_wins;
	carta player1[MAX_CARTE_X_PLAYER];
	carta banco[MAX_CARTE_X_PLAYER];
	
	/*nplayer = 1;*/
	flag = 'n';
	init_game();
	b_wins = 0;
	p1_wins = 0;
	/* Ciclo gioco 
	printf("Inserisci il numero di giocatori: ");
	scanf("%d", &nplayer);
	*/
	printf("Di quanto denaro disponi? ");
	scanf("%f", &portafoglio);
	while((flag!='y' && flag!='Y') && portafoglio > 0){
		/*print_var_status();*/
		gameOver = 0; /* 0=still playing; 1=stand; 2=sballa; 3=max/bj */
		p1_score = 0;
		b_score = 0;
		pp = 0;
		bp = 0;
		giro = 0;
		hitorstand = 'O';
		assicurazione = 'N';

		printf("Inserisci l'importo da puntare: ");
		scanf("%f", &puntata);
		portafoglio = portafoglio - puntata;
		printf("Nulla va più, in bocca al lupo!\n");
		while(!gameOver){
			if(giro<2){ /*Nei primi 2 giri le carte vengono distribuite e basta*/
				player1[pp] = deal(mazzo);
				banco[bp] = deal(mazzo);
				pp++;
				bp++;
			}else if(giro==2){/*Al secondo giro ti vengono fatte vedere le carte*/
				/*Stato PLAYER 1*/
				printf("Le tue carte:\n");
				printMazzo(player1, pp);
				p1_score = evaluateMazzo(player1, pp);
				printf("Hai %d punti nella mano\n", p1_score);
				/*Stato mazzo*/
				printf("Il banco:\n");
				printMazzoNascosto(banco, bp);
				b_score = evaluateMazzo(banco, bp);
				/*Se il banco ha come carta scoperta un asso, chiedi al giocatore se vuole fare l'assicurazione*/
				if(banco[0].nome == 'A'){
					printf("Il banco rischia di fare blackjack! Vuoi fare l'assicurazione? [Y/N]");
					scanf(" %c", &assicurazione);
					if(assicurazione == 'y' ||assicurazione == 's' || assicurazione == 'Y' || assicurazione == 'S'){
						portafoglio = portafoglio - (puntata/2.0);
					}
				}
			}else{ /*If you're on round 3 or more*/
				/*Scelta: dai o stai*/
				if(b_score == 21 && isBlackJack(banco, bp))
					gameOver = 1;
				else if(p1_score == 21)
					gameOver = 3;
				else{
					printf("[H]it or [S]tand? [H/S] [D/S]");
					scanf(" %c", &hitorstand);
					if(hitorstand == 'h' || hitorstand == 'd' || hitorstand == 'H' || hitorstand == 'D'){
						player1[pp] = deal(&mazzo[0]);
						pp++;
						p1_score = evaluateMazzo(player1, pp);
						/*Andare avanti a oltranza e valutare sempre se p1 sfora*/
						if(p1_score > 21){
							if(!assoInMazzo(player1, pp)){
								/*printf("Le tue carte:\n");
								printMazzo(player1, pp);*/
								gameOver = 2;
							}else{
								assoValeUno(&player1[0], pp);
								p1_score = evaluateMazzo(player1, pp);
							}
						}else if(p1_score == 21){
							/*printf("Le tue carte:\n");
							printMazzo(player1, pp);*/
							gameOver = 1;
						}else{
							/*printf("Le tue carte:\n");
							printfMazzo(player1, pp);*/
						}
						/*Ulteriore valutazione del mazzo*/
						p1_score = evaluateMazzo(player1, pp);
						printf("Le tue carte:\n");
						printMazzo(player1, pp);
						printf("I tuoi punti: %d\n", p1_score);
					}else if(hitorstand == 'S' || hitorstand == 's'){
						p1_score = evaluateMazzo(player1, pp);
						gameOver = 1;
					}
				}
			}
			giro++;
		}
		/*È finito il giro, si vede il risultato*/
		/*CONDIZIONI DI GAMEOVER*/
		if(gameOver == 2){
			/*p1 sballa, il banco vince (p1 solo al tavolo)*/
			b_score = evaluateMazzo(banco, bp);
			printf("Il banco:\n");
			printMazzo(banco, bp);
			printf("LOSE! Hai sballato e perso %f euro!\n", puntata);
		/*Se il player non ha sballato*/
		}else{
			/*Continua il banco con le sue regole*/
			/*Se il banco ha blackjack di base vince, il giocatore può fare solamente push solo se ha fatto l'assicurazione*/
			if(isBlackJack(banco, bp)){
				printf("Il banco:\n");
				printMazzo(banco, bp);
				if((assicurazione == 'y' || assicurazione == 'Y')  && isBlackJack(player1, pp)){
					portafoglio = portafoglio + puntata;
					printf("PUSH! Non vinci né perdi niente\n");
				}else{
					printf("LOSE! Hai perso! Il banco ha blackjack\n");
					b_wins++;
				}
			/*Se il banco non ha fatto bj*/
			}else{
				/*Se il giocatore ha blackjack, viene pagato 3 a 2*/
				if(isBlackJack(player1, pp)){
					printf("WIN! Winner winner chicken dinner! Hai fatto blackjack!\n");
					printf("Il banco aveva:\n");
					printMazzo(banco, bp);
					portafoglio += puntata*3/2;
					p1_wins++;
				}else{
					/*In tutti gli altri casi, il banco chiede carta finché non arriva a 17, dopodiché si ferma*/
					/*Problema che si verifica quando il banco ha un asso in mano è che non lo considera come 1 quando sballa*/
					if(b_score > 21 && assoInMazzo(banco, bp))
						assoValeUno(&banco[0], bp);
					while(b_score<17){
						printf("Il banco:\n");
						printMazzo(banco, bp);

						banco[bp] = deal(mazzo);
						bp++;
						b_score = evaluateMazzo(banco, bp);
						if(b_score > 21 && assoInMazzo(banco, bp))
							assoValeUno(&banco[0], bp);
						b_score = evaluateMazzo(banco, bp);
					}
					/*Confronto del risultato con quello del giocatore 1*/
					printf("Il banco:\n");
					printMazzo(banco, bp);
					b_score = evaluateMazzo(banco, bp);/*Dovrebbe essere inutile ma last resort*/
					if(b_score > 21){
						portafoglio += 2*puntata;
						printf("WIN! Il banco sballa, bella per voi!\n");
						p1_wins++;
					}else if(b_score > p1_score){
						printf("LOSE! Peccato! Il banco ha totalizzato %d punti, contro i tuoi %d. Ciò significa che hai perso.\n", b_score, p1_score);
						b_wins++;
					}else if(b_score < p1_score){
						portafoglio += 2*puntata;
						printf("WIN! Bella per P1 che totalizza %d punti e il banco solo %d, vincendo i soldi!\n", p1_score, b_score);
						p1_wins++;
					}else if(b_score == p1_score){
						portafoglio += puntata;
						printf("PUSH! Non hai vinto né perso in quanto sia tu che il banco avete totalizzato %d punti!\n", b_score);
					}
				}
			}
		}
		printf("Il tuo bilancio è di %f euro\n", portafoglio);
		printf("VUOI USCIRE? [Y/N] (default=no)");
		scanf(" %c", &flag);
	}
	return 0;
}

