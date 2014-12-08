

/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}




void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd, port;
    
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

	/* Determine the domain name and IP address of the client */
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	haddrp = inet_ntoa(clientaddr.sin_addr);
	printf("server connected to %s (%s)\n", hp->h_name, haddrp);


	char buf[MAXLINE];
	rio_t rio;
	Rio_readinitb(&rio, connfd);
	Rio_readlineb(&rio, buf, MAXLINE);

	char **tokens;
	tokens = str_split(buf, ' ');
	if(strcmp("mcput", tokens[0]) == 0)
	{
	  printf("Request Type = put\n");
	  //do put stuff
	}
	else if("mcget", tokens[0])
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
