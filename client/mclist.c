//mclist sends request to list all files on the cloud server

#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  int port, clientfd;
  char data[CONTENT_MAX];
  //unsigned int size = fread(data, sizeof(char), CONTENT_MAX, stdin);
  int type = LIST;
  char host[HOST_LENGTH];
  unsigned int secret_key;
  char filename[FILENAME_MAX];
  char *buf = malloc(LIST_REQ_HEADER);
  memset(buf, 0, LIST_REQ_HEADER);

  if(argc != 4)
  {
    printf("usage: ./mcdel <MachineName> <port> <key>\n");
    return 0;
  }
  
  strcpy(host, argv[1]);
  port = atoi(argv[2]);
  secret_key = atoi(argv[3]);
  strcpy(filename, argv[4]);
  
  //rio_t rio; 
  clientfd = Open_clientfd(host, port); 

  port = htonl(port);
  secret_key = htonl(secret_key);

  memcpy(buf, &secret_key, 4);
  memcpy(buf+4, &type, 4);
  
  //Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, LIST_REQ_HEADER);
  Rio_readnb(clientfd, buf, LIST_RESP_HEADER);

  Close(clientfd);
  exit(0);
}

