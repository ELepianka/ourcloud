//mclist sends a request to put a file onto the cloud server

#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  int port, clientfd;
  char* data = malloc(CONTENT_MAX);
  int type = LIST;
  char host[HOST_LENGTH];
  unsigned int secret_key;
  char *buf = malloc(PUT_REQ_HEADER+CONTENT_MAX);
  memset(buf, 0, PUT_REQ_HEADER+CONTENT_MAX);

  if(argc != 4)
  {
    printf("usage: ./mclist <MachineName> <port> <key>\n");
    return 0;
  }
  
  char* response = malloc(PUT_REQ_HEADER+CONTENT_MAX);
  memset(response, 0, PUT_REQ_HEADER+CONTENT_MAX);
  
  strcpy(host, argv[1]);
  port = atoi(argv[2]);
  secret_key = atoi(argv[3]);
  
  rio_t rio; 
  clientfd = Open_clientfd(host, port); 

  port = htonl(port);
  secret_key = htonl(secret_key);
  type = htonl(type);

  memcpy(buf, &secret_key, 4);
  memcpy(buf+4, &type, 4);
  
  Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, PUT_REQ_HEADER+CONTENT_MAX);
  Rio_readnb(&rio, response, PUT_REQ_HEADER+CONTENT_MAX);

  int status; //-1 is an error, 0 is success
  memcpy(&status,response, 4);
  status = htonl(status);
  
  if (status == 0){printf("Operation Status: success\n");}
  else if(status == -1){printf("Error storing file\n");}

  int size;
  memcpy(&size, response+4, 4);
  size = ntohl(size);

  memcpy(data, response+4+4, size);
  fwrite(data, sizeof(char), size, stdout);

  free(buf);
  free(response);
  Close(clientfd);
  exit(0);
}

