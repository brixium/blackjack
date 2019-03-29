#include <time.h>
#include "libbj.h"


int main(int argc, char * argv[]){
	char flag;
	carta mazzo[DIMMAZZO];
	clock_t start, end;
	double delta;
	
	srand(time(NULL));
	start = clock();

	flag = 'n';
	init_game();
	init_mazzo(mazzo);
	printMazzo(mazzo);
	printf("\nNuovo mazzo:\n");

	mischiaMazzo(&mazzo[0]);
	printMazzo(mazzo),

	print_var_status();
	
	/*
	printf("Di quanto denaro disponi?  ");
	scanf("%d", &portafoglio);
	while(flag=='n' || flag=='N'){
		printf("Inserisci l'importo da puntare: ");
		scanf("%d", &puntata);
		
		printf("VUOI USCIRE? [Y/n] (default=yes)");
		scanf("%c", &flag);
	}
	*/
	end = clock();
	delta = end-start;
	printf("\nExec time: 0,%.0f s\n", delta);
	return 0;
}
