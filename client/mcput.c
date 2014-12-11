#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  printf("Input file is read from stdin\n");
  int port, clientfd;
  char data[CONTENT_MAX];
  int size = (int)(fread(data, sizeof(char), CONTENT_MAX, stdin));
  printf("size: %d\n", size);
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
  
  char* response = malloc(PUT_RESP_HEADER);
  memset(response, 0, PUT_RESP_HEADER);
  
  strcpy(host, argv[1]);
  port = atoi(argv[2]);
  secret_key = atoi(argv[3]);
  strcpy(filename, argv[4]);
  
  rio_t rio; 
  clientfd = Open_clientfd(host, port); 

  port = htonl(port);
  secret_key = htonl(secret_key);
  type = htonl(type);
  size = htonl(size);

  memcpy(buf, &secret_key, 4);
  memcpy(buf+4, &type, 4);
  memcpy(buf+4+4, &filename, FILENAME_MAX);
  memcpy(buf+88, &size, 4);
  size = ntohl(size);
  memcpy(buf+4+4+FILENAME_MAX+4, &data, size);
  
  Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, PUT_REQ_HEADER+CONTENT_MAX);
  Rio_readnb(&rio, response, PUT_RESP_HEADER);

  int status; //-1 is an error, 0 is success
  memcpy(&status,response, 4);
  status = htonl(status);

  if (status == 0){printf("Operation Status: success\n");}
  else if(status == -1){printf("Error storing file\n");}


  free(buf);
  free(response);
  Close(clientfd);
  exit(0);
}
