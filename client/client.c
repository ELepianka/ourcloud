/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char **argv) {

    int clientfd, port, size;
    char *host;
    char buf[100*1024];
    char command_type;
    rio_t rio;

    if (argc < 5) {
	fprintf(stderr, "usage: %s <command> <host> <port> <secret> optional<filename>\n", argv[0]);
	exit(0);
    }
    host = argv[2];
    port = atoi(argv[3]);
    command_type = atoi(argv[1]);

    clientfd = Open_clientfd(host, port); //creates a socket, creates an address, 
					  //and establishes conenction
    Rio_readinitb(&rio, clientfd);	  //initializes rio

    if(command_type == PUT){
	size = fread(buf,sizeof(char),100*1024,stdin);
	buf[size] = 0;
	size += 1;
    }

      Rio_writen(clientfd, buf, size);
      Rio_readn(clientfd, buf, size);


    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */