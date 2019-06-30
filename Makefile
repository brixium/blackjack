CC=gcc
CFLAGS=-Wall -O2 -std=c89 -pedantic -static

blackjack: blackjack.o cards.o money.o
	$(CC) $(CFLAGS) bj.o cards.o money.o -o blackjack
blackjack.o: bj.c cards.h money.h
	$(CC) -c bj.c -o bj.o
cards.o: cards.c cards.h
	$(CC) -c cards.c -o cards.o
money.o: money.c money.h
	gcc -c money.c -o money.o

clean:
	rm *.o
