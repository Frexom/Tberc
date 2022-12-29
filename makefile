CC=gcc
CFLAGS=-Wall -Werror -lpthread


#Compilation serveur 1 ou 2
server2: server2.o
	$(CC) bin/server2.o -o exec/server2

server2.o: files/server2.c
	$(CC) -c files/server2.c -o bin/server2.o $(CFLAGS)

server1: server1.o
	$(CC) bin/server1.o -o exec/server1

server1.o: files/server1.c
	$(CC) -c files/server1.c -o bin/server1.o $(CFLAGS)


client1: client1.o
	$(CC) bin/client1.o -o exec/client1

client1.o: files/client1.c
	$(CC) -c files/client1.c -o bin/client1.o $(CFLAGS)

client2: client2.o
	$(CC) bin/client2.o -o exec/client2

client2.o: files/client2.c
	$(CC) -c files/client2.c -o bin/client2.o $(CFLAGS)


#Nettoyer avec 'make clean'
clean:
	rm bin/*.o
