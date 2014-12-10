CC=gcc -Wall -std=gnu99
CFLAGS=-I.
OBJECTS = server.o mcput.o csapp.o
HEADERS = csapp.h options.h

all: ourserver mcput

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

ourserver: include/csapp.o server/server.o
	${CC} $^ -lpthread -Icommon -o $@

mcput: include/csapp.o client/mcput.o
	${CC} $^ -lpthread -Icommon -o $@

clean:
	rm -f client/mcput.o
	rm -f server/server.o
	rm -f include/csapp.o
