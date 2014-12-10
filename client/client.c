#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char **argv) {

    int clientfd, port;
    int size = 0;
    char *host;
    char buf[CONTENT_MAX];
    int command_type;
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
	
    size = sizeof(char)*CONTENT_MAX;
    buf[size] = 0;
    size += 1;

    printf("%d\n", command_type);

    if(command_type == PUT){
	printf("CLIENT: PUT\n");
    }
    else if(command_type == GET){
      printf("CLIENT: GET\n");
    }
    else if(command_type == DELETE){
	printf("CLIENT: DELETE\n");
    }
    else if(command_type == LIST){
	printf("CLIENT: LIST\n");
    }

      Rio_writen(clientfd, buf, size);
      Rio_readn(clientfd, buf, size);


    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
