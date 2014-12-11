#include "../include/csapp.h"
#include "../include/options.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(int argc, char **argv) {

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

    	char* buf;
        size_t size;
    	size = sizeof(char)*(CONTENT_MAX);
    	rio_t rio;
    	Rio_readinitb(&rio, connfd);
        char* tmp = malloc(PUT_REQ_HEADER+CONTENT_MAX);
        int user_key = 0;
        int tmpint = 0;       
        rio_readnb(&rio, tmp, PUT_REQ_HEADER+CONTENT_MAX);
	memcpy(&user_key, tmp, 4);
        user_key = ntohl(user_key);
        printf("read something!\n%s\n",tmp);
        if (user_key != secret_key){
            return -1;
        }
	int request_type = -1;
	memcpy(&request_type, tmp+4, 4);
	request_type = ntohl(request_type);
	if(request_type == -1)
	{
	  printf("Poorly formatted request\n");
	}
	if(request_type == 1)
	{
	  printf("type: PUT\n");
	}

//        Rio_readnb(&rio, tmp, 4);
//        tmpint = atoi(tmp);
//        if (tmpint != 1){
//           return -1;
//        }       

        exit(0);
    }
}
