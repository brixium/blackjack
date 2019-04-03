#include <time.h>
#include "libbj.h"


int main(int argc, char * argv[]){
	char flag, scelta;
	/*int nplayer;*/
	int giro, pp, bp, gameOver, p1_score, b_score;
	carta mazzo[DIMMAZZO];
	carta player1[MAX_CARTE_X_PLAYER];
	carta banco[MAX_CARTE_X_PLAYER];
	clock_t start, end;
	double delta;
	
	srand(time(NULL));
	start = clock();
	/*nplayer = 1;*/
	flag = 'n';
	init_game();
	init_mazzo(mazzo);
	mischiaMazzo(&mazzo[0]);
	print_var_status();
	
	/* Ciclo gioco 
	printf("Inserisci il numero di giocatori: ");
	scanf("%d", &nplayer);
	*/
	printf("Di quanto denaro disponi? ");
	scanf("%d", &portafoglio);
	while((flag=='n' || flag=='N') && portafoglio > 0){
		gameOver = 0; /* 0=still playing; 1=stand; 2=sballa; 3=max/bj */
		p1_score = 0;
		b_score = 0;
		pp = 0;
		bp = 0;
		giro = 0;
		scelta = 'O';

		printf("Inserisci l'importo da puntare: ");
		scanf("%d", &puntata);
		printf("Nulla va più, in bocca al lupo!\n");
		while(!gameOver){
			if(giro<2){
				player1[pp] = deal(mazzo);
				banco[bp] = deal(mazzo);
				pp++;
				bp++;
			}
			if(giro>=2){
				/*Stato PLAYER 1*/
				printf("Le tue carte:\n");
				printMazzo(player1, pp);
				p1_score = evaluateMazzo(player1, pp);
				printf("Hai %d punti nella mano\n", p1_score);
				/*Stato mazzo*/
				printf("Il banco:\n");
				printMazzo(banco, bp);
				b_score = evaluateMazzo(banco, bp);
				printf("Il banco ha %d punti\n", b_score);
				/*Scelta: dai o stai*/
				printf("[D]ai carta oppure [S]tai? [D/S] ");
				scanf(" %c", &scelta);
				if(scelta == 'D' || scelta == 'd'){
					player1[pp] = deal(mazzo);
					pp++;
					p1_score = evaluateMazzo(player1, pp);
					/*Andare avanti a oltranza e valutare sempre se p1 sfora*/
					if(p1_score >21){
						if(!assoInMazzo(player1, pp))
							gameOver = 2;
						else{
							assoValeUno(&player1[0], pp);
						}
					}else if(p1_score == 21)
						gameOver = 3;
					/*Ulteriore valutazione del mazzo*/
					p1_score = evaluateMazzo(player1, pp);
					printf("I tuoi punti: %d\n", p1_score);
				}else if(scelta == 'S' || scelta == 's'){
					p1_score = evaluateMazzo(player1, pp);
					gameOver = 1;
				}
			}
			giro++;
		}
		/*Qui vedere le condizioni di gameOver e fare proseguire il gioco del mazzo*/
		printf("VUOI USCIRE? [Y/n] (default=yes)");
		scanf(" %c", &flag);
	}
	
	end = clock();
	delta = end-start;
	printf("\nExec time: %f ms\n", delta);
	return 0;
}
