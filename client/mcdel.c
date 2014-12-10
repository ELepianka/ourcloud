//mcdel sends request to delete a file on the cloud server

#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  int port, clientfd;
  char data[CONTENT_MAX];
  //unsigned int size = fread(data, sizeof(char), CONTENT_MAX, stdin);
  int type = DEL;
  char host[HOST_LENGTH];
  unsigned int secret_key;
  char filename[FILENAME_MAX];
  char *buf = malloc(DEL_REQ_HEADER);
  memset(buf, 0, DEL_REQ_HEADER);

  if(argc != 5)
  {
    printf("usage: ./mcdel <MachineName> <port> <key> <filename>\n");
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
  
  //Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, DEL_REQ_HEADER);
  Rio_readnb(clientfd, data, DEL_RESP_HEADER);

  int status = -1; //-1 is an error, 0 is success
  memcpy(data, status, 4);
  status = ntohl(status);

  if(status == -1)
  {
    printf("Error storing file\n")
  }

  Close(clientfd);
  exit(0);
}

