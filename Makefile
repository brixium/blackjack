CC=gcc
CFLAGS=-Wall -O2 -std=c89 -pedantic -static
CFLAGSNS=-Wall -O2 -std=c89 -pedantic
DIRMONEY=money/
DIRCARDS=cards/
DIRPLAYER=player/

blackjack_static: blackjack.o $(DIRCARDS)cards.o $(DIRMONEY)money.o $(DIRPLAYER)player.o
	$(CC) $(CFLAGS) bj.o $(DIRCARDS)cards.o $(DIRMONEY)money.o $(DIRPLAYER)player.o -o blackjack
blackjack: blackjack.o $(DIRCARDS)cards.o $(DIRMONEY)money.o $(DIRPLAYER)player.o
	$(CC) $(CFLAGSNS) bj.o $(DIRCARDS)cards.o $(DIRMONEY)money.o $(DIRPLAYER)player.o -o blackjack

blackjack.o: bj.c $(DIRCARDS)cards.h $(DIRMONEY)money.h
	$(CC) -c bj.c -o bj.o
cards.o: $(DIRCARDS)cards.c $(DIRCARDS)cards.h
	$(CC) -c $(DIRCARDS)cards.c -o cards.o
money.o: $(DIRMONEY)money.c $(DIRMONEY)money.h
	$(CC) -c $(DIRMONEY)money.c -o money.o
player.o: $(DIRPLAYER)player.c $(DIRPLAYER)player.h
	$(CC) -c $(DIRPLAYER)player.c -o $(DIRPLAYER)player.o

clean:
	rm *.o $(DIRMONEY)*.o $(DIRCARDS)*.o $(DIRPLAYER)*.o
