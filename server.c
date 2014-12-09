/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd, port, secret_key;
    
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    if (argc < 3) {
	fprintf(stderr, "usage: %s <port> <secret>\n", argv[0]);
	exit(0);
    }
    port = atoi(argv[1]);
    secret_key = atoi(argv[2]);

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

	/* Determine the domain name and IP address of the client */
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	haddrp = inet_ntoa(clientaddr.sin_addr);
	printf("server connected to %s (%s)\n", hp->h_name, haddrp);





	char buf[1024*100];
	int size;
size = sizeof(char),100*1024;
	rio_t rio;
	Rio_readinitb(&rio, connfd);
//	Rio_readn(&rio, buf, 100*1024);

	char **tokens;
//	tokens = str_split(buf, ' ');

//	if(strcmp("mcput", tokens[0]) == 0)
	{
	  printf("Request Type = put\n");
	  Rio_writen(&rio, buf, size);
	  //do put stuff
	}
	if("mcget", tokens[0])
	{
	  printf("Request Type = get\n");
	  //do get stuff
	}
	
	else if("mcdel", tokens[0])
	{
	  printf("Request Type = del\n");
	  //
	}
	else if("mclist", tokens[0])
	{
	  printf("Request Type = list\n");
	}
	//echo(connfd);

	Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
