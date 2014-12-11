#include "../include/csapp.h"
#include "../include/options.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include<unistd.h>

void connection(socklen_t clientlen, struct sockaddr_in clientaddr){
    struct hostent *hp;
    char *haddrp;
    
    /* Determine the domain name and IP address of the client */
    hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
    sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    haddrp = inet_ntoa(clientaddr.sin_addr);
    printf("server connected to %s (%s)\n", hp->h_name, haddrp);
}
/*
struct files{
    char filename[FNAME_MAX];
    struct files *next;
};
*/
int main(int argc, char **argv) {

    int listenfd, connfd, port; 
    unsigned int secret_key;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
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
        connection(clientlen,clientaddr);

        struct files *root;
        struct files *conductor;  
        root = malloc(sizeof(struct files));  
        root->next = 0;
        
        strcpy(root->filename,"herpaderp");
//        conductor = root;
//        while (conductor != NULL){
//            printf("%s\n", conductor->filename);
//            conductor = conductor->next;
//        }

    	rio_t rio;
    	Rio_readinitb(&rio, connfd);
        char* tmp = malloc(PUT_REQ_HEADER+CONTENT_MAX);
        int user_key = 0;
        rio_readnb(&rio, tmp, PUT_REQ_HEADER+CONTENT_MAX);
    	memcpy(&user_key, tmp, 4);//user_key = bytes 0-3
        user_key = ntohl(user_key);
        if (user_key != secret_key){
	        printf("incorrect key\n");
            return -1;
        }
        else {
            printf("Secret Key = %d\n",user_key);
        }

	    unsigned int request_type = 5;
        memcpy(&request_type, tmp+4, 4); //type = bytes 4-7
	    request_type = ntohl(request_type);
        if(request_type == 5){
	        printf("Poorly formatted request\n");
    	}

        int resp = 0;
        char filename[FNAME_MAX];
//        char path[10+FNAME_MAX];
        char* buf = malloc(CONTENT_MAX);
        int size = 0;
        switch(request_type){
            case 0:
                printf("Request Type = get\n");
                memcpy(filename, tmp+4+4, 80); //filename = bytes 8-87
                printf("Filename = %s\n",filename);
                char data[CONTENT_MAX];
                printf("just before the fread\n");
                FILE *file;
                file = fopen(filename,"w");
                size = (int)(fread(data, sizeof(char), CONTENT_MAX, file));
                fclose(file);
                printf("just after the fread\n");
                char *buf = malloc(8+CONTENT_MAX);
/*                memset(buf, 0, 8+CONTENT_MAX);
                memcpy(buf, &resp, 4);
                memcpy(buf+4, &size, 4);
                memcpy(buf+4+4, &data, size);
                Rio_writen(connfd,buf,8+CONTENT_MAX);
*/                Rio_writen(connfd,&resp,4);
                break;
            case 1:
                printf("Request Type = put\n");
                memcpy(filename, tmp+4+4, 80); //filename = bytes 8-87
                printf("Filename = %s\n",filename);
        
                strcpy(root->filename,filename);
 
                memcpy(&size,tmp+88,4);
                size = ntohl(size);
//                printf("Size of content: %d\n",size);
//                strcpy(path,"../server/");
//                printf("path = %s\n",path);
//                strcat(path,filename);
//                printf("path = %s\n",path);
                memcpy(buf,tmp+92,size);
                FILE *fp;
                fp = fopen(filename,"w");
                fwrite(buf,sizeof(char),size,fp);
                fclose(fp);

                Rio_writen(connfd,&resp,4);
                break;
            case 2:
                printf("Request Type = del\n");
                memcpy(filename, tmp+4+4, 80); //filename = bytes 8-87
                printf("Filename = %s\n",filename);
                remove(filename);
                break;
            case 3:
                printf("Request Type = list\n");
                conductor = root;
/*                while (conductor != NULL){
                    printf("%s\n", conductor->filename);
                    conductor = conductor->next;
                }
*/                break;
        };
	

        exit(0);
    }
}
