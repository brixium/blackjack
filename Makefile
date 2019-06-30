CC=gcc
CFLAGS=-Wall -O2 -std=c89 -pedantic -static
CFLAGSNS=-Wall -O2 -std=c89 -pedantic
DIRMONEY=money/
DIRCARDS=cards/

blackjack_static: blackjack.o $(DIRCARDS)cards.o $(DIRMONEY)money.o
	$(CC) $(CFLAGS) bj.o $(DIRCARDS)cards.o $(DIRMONEY)money.o -o blackjack
blackjack: blackjack.o $(DIRCARDS)cards.o $(DIRMONEY)money.o
	$(CC) $(CFLAGSNS) bj.o $(DIRCARDS)cards.o $(DIRMONEY)money.o -o blackjack

blackjack.o: bj.c $(DIRCARDS)cards.h $(DIRMONEY)money.h
	$(CC) -c bj.c -o bj.o
cards.o: $(DIRCARDS)cards.c $(DIRCARDS)cards.h
	$(CC) -c $(DIRCARDS)cards.c -o cards.o
money.o: $(DIRMONEY)money.c $(DIRMONEY)money.h
	gcc -c $(DIRMONEY)money.c -o money.o

clean:
	rm *.o $(DIRMONEY)*.o $(DIRCARDS)*.o
