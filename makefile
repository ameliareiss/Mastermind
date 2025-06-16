CC=gcc
CFlags=-g -O1 -Wall
LDLIBS=-lpthread

all: client server

client: client.c csapp.h csapp.c game.h game.c
server: server.c csapp.h csapp.c game.h game.c

clean:
	rm -f *.o *~ *.exe client server csapp.o