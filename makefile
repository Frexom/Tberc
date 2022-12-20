CC=gcc
CFLAGS=-Wall

#Run le projet
run: tberc
	@echo
	./tberc

#Compiler l'exécutable
tberc: main.o
	$(CC) bin/main.o -o tberc

#Compiler main.c en main.o
main.o: main.c
	$(CC) -c main.c -o bin/main.o $(CFLAGS)

#Nettoyer avec 'make clean'
clean:
	rm bin/*.o
