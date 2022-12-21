CC=gcc
CFLAGS=-Wall -Werror -lpthread

runServer: server
	@echo
	./exec/server

server: server.o
	$(CC) bin/server.o -o exec/server

server.o: files/server.c
	$(CC) -c files/server.c -o bin/server.o $(CFLAGS)

#Nettoyer avec 'make clean'
clean:
	rm bin/*.o
