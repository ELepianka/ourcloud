//mcput sends a request to put a file onto the cloud server

#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  printf("Input file is read from stdin\n");
  int port, clientfd;
  char data[CONTENT_MAX];
  unsigned int size = fread(data, sizeof(char), CONTENT_MAX, stdin);
  int type = PUT;
  char host[HOST_LENGTH];
  unsigned int secret_key;
  char filename[FILENAME_MAX];
  char *buf = malloc(PUT_REQ_HEADER+CONTENT_MAX);
  memset(buf, 0, PUT_REQ_HEADER+CONTENT_MAX);

  if(argc != 5)
  {
    printf("usage: ./mcput <MachineName> <port> <key> <filename>\n");
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
  memcpy(buf+4+4, filename, FILENAME_MAX); 
  memcpy(buf+4+4+FILENAME_MAX, &size, 4);
  memcpy(buf+4+4+FILENAME_MAX+4, &data, size);
  
  //Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, PUT_REQ_HEADER+CONTENT_MAX);
  Rio_readnb(clientfd, data, PUT_RESP_HEADER);

  int status = -1; //-1 is an error, 0 is success
  memcpy(data, status, 4);
  status = ntohl(status);

  if(status == -1)
  {
    printf("Error storing file\n");
  }

  Close(clientfd);
  exit(0);
}

