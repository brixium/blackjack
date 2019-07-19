char cli_cycle; /*former quit*/
char hitorstand; /*decide whether you stand or hit*/
char insurance;	/*y=do it; n=don't*/

int round;
int nplayer; /*Number of players*/
int round;	/*counts the number of phases in a round (0:*/
int pp;		/*n of cards in player's deck*/
int bp;		/*n of cards in dealer's deck*/
int gameOver;	/*How did the game end? 0=still playing; 1=the player stands; 2=player bust; 3=21 or bj */
int p1_score;	/*p1 deck points (pp/21)*/
int b_score;	/*dealer deck points (bp/21)*/
int b_wins;		/*how many times did the player won */
int p1_wins;	/*how many times the dealer won*/
