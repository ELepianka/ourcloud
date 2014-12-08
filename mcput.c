/*
 * mcput.c - A put program for the cloud
 */
/* $begin my cloud put */
#include "csapp.h"

int main(int argc, char **argv) 
{
    int clientfd, port;
    char *host; 
    char *buf;
    rio_t rio;
    //int key;
    char filename;
    int i = 0;

//    if (argc != 5) {
//	fprintf(stderr, "usage: %s <host> <port> <filename>\n", argv[0]);
//	exit(0);
  //  }
    host = argv[1];
    port = atoi(argv[2]);

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);


    for(i=0; i < 4; i++){
      printf("%s\n", argv[i]);
	strcat(buf, (argv[i]));
	strcat(buf,' ');
    }
printf("%s\n", buf);
    Rio_writen(clientfd, buf, strlen(buf));
    
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end my cloud put */
