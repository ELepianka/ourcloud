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
        printf("we're at the readinitb\n");
    	Rio_readinitb(&rio, connfd);
        char* tmp;
        int user_key = 0;
        int tmpint = 0;       
 
        printf("did not read anything!\n");
        Rio_readnb(&rio, tmp, 4);
        user_key = atoi(tmp);
        printf("read something!\n%d\n",user_key);
        if (user_key != secret_key){
            return -1;
        }
//        Rio_readnb(&rio, tmp, 4);
//        tmpint = atoi(tmp);
//        if (tmpint != 1){
//           return -1;
//        }       

        exit(0);
    }
}
