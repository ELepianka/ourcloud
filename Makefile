CC=gcc -Wall -std=gnu99
CFLAGS=-I.
OBJECTS = server.o client.o csapp.o
HEADERS = csapp.h options.h

all: ourserver ourclient

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

ourserver: include/csapp.o server/server.o
	${CC} $^ -lpthread -Icommon -o $@

ourclient: include/csapp.o client/client.o
	${CC} $^ -lpthread -Icommon -o $@

clean:
	rm -f client/client.o
	rm -f server/server.o
