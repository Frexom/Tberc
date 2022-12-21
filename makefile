CC=gcc
CFLAGS=-Wall -Werror -lpthread

server: server.o
	$(CC) bin/server.o -o exec/server

server.o: files/server.c
	$(CC) -c files/server.c -o bin/server.o $(CFLAGS)

client: client.o
	$(CC) bin/client.o -o exec/client

client.o: files/client.c
	$(CC) -c files/client.c -o bin/client.o $(CFLAGS)

#Nettoyer avec 'make clean'
clean:
	rm bin/*.o
