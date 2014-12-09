CC=gcc -Wall -std=gnu99
CFLAGS=-I.
OBJECTS = server.o client.o csapp.o
HEADERS = csapp.h options.h

all: server client

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: csapp.o server.o
	${CC} $^ -lpthread -Icommon -o $@

client: csapp.o client.o
	${CC} $^ -lpthread -Icommon -o $@

clean:
	rm -f $(OBJECTS)
