CC = gcc
CFLAGS = -Wall 

#generazione exe
esame: compito.o tipo.o liste.o
	$(CC) $(CFLAGS) -o esame $^

#generazione file oggetto
compito.o: compito.c liste.h tipo.h
	$(CC) $(CFLAGS) -c $^

tipo.o: tipo.c tipo.h
	$(CC) $(CFLAGS) -c $^

liste.o: liste.c liste.h
	$(CC) $(CFLAGS) -c $^