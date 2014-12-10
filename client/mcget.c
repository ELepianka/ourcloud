//mcget sends request for a file on the cloud server

#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  int port, clientfd;
  char response[CONTENT_MAX];
  int type = GET;
  char host[HOST_LENGTH];
  unsigned int secret_key;
  char filename[FILENAME_MAX];
  char *buf = malloc(GET_REQ_HEADER);
  memset(buf, 0, GET_REQ_HEADER);

  if(argc != 5)
  {
    printf("usage: ./mcget <MachineName> <port> <key> <filename>\n");
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
  
//  Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, GET_REQ_HEADER);
  Rio_readnb(clientfd, response, GET_RESP_HEADER);

  int status = -1; //-1 is an error, 0 is success
  int size = 0;
  memcpy(response, status, 4);
  status = ntohl(status);

  if(status == -1)
  {
    printf("Error storing file\n");
  }

  memcpy(response+4, size, 4);
  size = ntohl(size);
  if(size > CONTENT_MAX)
  {
    printf("Response too large\n");;
  }
  else
  {
    char *data = malloc(size);
    fwrite(data, sizeof(char), size, stdout);
  }

  Close(clientfd);
  exit(0);
}

