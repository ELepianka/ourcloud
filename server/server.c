#include "../include/csapp.h"
#include "../include/options.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

void connection(socklen_t clientlen, struct sockaddr_in clientaddr){
    struct hostent *hp;
    char *haddrp;
    
    /* Determine the domain name and IP address of the client */
    hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
    sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    haddrp = inet_ntoa(clientaddr.sin_addr);
    printf("server connected to %s (%s)\n", hp->h_name, haddrp);
}

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

    	rio_t rio;
    	Rio_readinitb(&rio, connfd);
        char* tmp = malloc(PUT_REQ_HEADER+CONTENT_MAX);
        int user_key = 0;
        rio_readnb(&rio, tmp, PUT_REQ_HEADER+CONTENT_MAX);
    	memcpy(&user_key, tmp, 4);//user_key = bytes 0-3
        user_key = ntohl(user_key);
	int resp = 0;
        if (user_key != secret_key){
	        printf("incorrect key\n");
		resp = -1;
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

        char filename[FNAME_MAX];
        char path[10+FNAME_MAX];  //used for opening files in different directories
        char* buf = malloc(CONTENT_MAX);
        int size = 0;
        switch(request_type){
            case 0:
                printf("Request Type = get\n");
                memcpy(filename, tmp+4+4, 80); //filename = bytes 8-87
                printf("Filename = %s\n",filename);
                strcpy(path,"server//");
                strcat(path,filename);
                char data[CONTENT_MAX];
                FILE *file;
                file = fopen(path,"r");
                fseek(file,0,SEEK_END);
                size = ftell(file);
                fseek(file,0,SEEK_SET);
                fclose(file);
                char *content = malloc(8+CONTENT_MAX);
                memset(content, 0, 8+CONTENT_MAX);
                memcpy(content, &resp, 4);
                memcpy(content+4, &size, 4);
                memcpy(content+4+4, &data, size);
                Rio_writen(connfd,content,8+CONTENT_MAX);
                break;
            case 1:
                printf("Request Type = put\n");
                memcpy(filename, tmp+4+4, 80); //filename = bytes 8-87
                printf("Filename = %s\n",filename);
        
 
                memcpy(&size,tmp+88,4);
                size = ntohl(size);
                strcpy(path,"server//");
                strcat(path,filename);
                memcpy(buf,tmp+92,size);
                FILE *fp;
                fp = fopen(path,"w");
                fwrite(buf,sizeof(char),size,fp);
                fclose(fp);

                Rio_writen(connfd,&resp,4);
                break;
            case 2:
                printf("Request Type = del\n");
                memcpy(filename, tmp+4+4, 80); //filename = bytes 8-87
                printf("Filename = %s\n",filename);
                strcpy(path,"server//");
                strcat(path,filename);
                remove(path);
                break;
            case 3:
                printf("Request Type = list\n");
		system("ls server/ > listfile.txt");
		char* listresponse = malloc(PUT_REQ_HEADER+CONTENT_MAX);
		memset(listresponse, 0, PUT_REQ_HEADER+CONTENT_MAX);
		FILE* listfile = fopen("listfile.txt", "r");
                
		fseek(listfile,0,SEEK_END);
                size = ftell(listfile);
                fseek(listfile,0,SEEK_SET);
		size = htonl(size);
                memcpy(listresponse, &resp, 4);
                memcpy(listresponse+4, &size, 4);
		size = ntohl(size);
		fread(listresponse+4+4, sizeof(char), size, listfile);
		Rio_writen(connfd, listresponse, PUT_REQ_HEADER+CONTENT_MAX);
		fclose(listfile);
                break;
        };
	

        exit(0);
    }
}
