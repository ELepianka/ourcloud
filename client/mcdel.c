#include <stdio.h>
#include "../include/csapp.h"
#include "../include/options.h"

int main(int argc, char** argv)
{
  int port, clientfd;
  

  int type = DEL;
  char host[HOST_LENGTH];
  unsigned int secret_key;
  char filename[FNAME_MAX];
  char *buf = malloc(PUT_REQ_HEADER+CONTENT_MAX);
  memset(buf, 0, PUT_REQ_HEADER+CONTENT_MAX);

  if(argc != 5)
  {
    printf("usage: ./mcput <MachineName> <port> <key> <filename>\n");
    return 0;
  }
  
  char* response = malloc(DEL_RESP_HEADER);
  memset(response, 0, DEL_RESP_HEADER);
  
  strcpy(host, argv[1]);
  port = atoi(argv[2]);
  secret_key = atoi(argv[3]);
  strcpy(filename, argv[4]);
  
  rio_t rio; 
  clientfd = Open_clientfd(host, port); 

  port = htonl(port);
  secret_key = htonl(secret_key);
  type = htonl(type);


  memcpy(buf, &secret_key, 4);
  memcpy(buf+4, &type, 4);
  memcpy(buf+4+4, &filename, FNAME_MAX);
  
  Rio_readinitb(&rio, clientfd);
  Rio_writen(clientfd, buf, PUT_REQ_HEADER+CONTENT_MAX);
  Rio_readnb(&rio, response, DEL_RESP_HEADER);

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

