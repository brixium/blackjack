#include "libbj.h"



int main(int argc, char * argv[]){
	char flag;
	carta mazzo[DIMMAZZO];
	
	flag = 'n';
	init_game();
	init_mazzo(mazzo);
	printMazzo(mazzo);
	print_var_status();
	
	printf("Di quanto denaro disponi?  ");
	scanf("%d", &portafoglio);
	while(flag=='n' || flag=='N'){
		printf("Inserisci l'importo da puntare: ");
		scanf("%d", &puntata);
		
		printf("VUOI USCIRE? [Y/n] (default=yes)");
		scanf("%c", &flag);
	}
	return 0;
}
