CC=gcc -Wall -std=gnu99
CFLAGS=-g -I.
OBJECTS = server.o mcput.o mcget.o mcdel.o mclist.o csapp.o
HEADERS = csapp.h options.h

all: ourserver mcput mcget mcdel mclist

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

ourserver: include/csapp.o server/server.o
	${CC} $^ -lpthread -Icommon -o $@

mcput: include/csapp.o client/mcput.o
	${CC} $^ -lpthread -Icommon -o $@

mcget: include/csapp.o client/mcget.o
	${CC} $^ -lpthread -Icommon -o $@

mcdel: include/csapp.o client/mcdel.o
	${CC} $^ -lpthread -Icommon -o $@

mclist: include/csapp.o client/mclist.o
	${CC} $^ -lpthread -Icommon -o $@

clean:
	rm -f client/mcput.o client/mcget.o client/mcdel.o client/mclist.o 
	rm -f server/server.o include/csapp.o
	rm -f ourserver mcput mcget mcdel mclist
