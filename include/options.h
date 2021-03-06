#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FNAME_MAX 80
#define CONTENT_MAX 100*1024
#define HOST_LENGTH 40
#define PUT_REQ_HEADER 93
#define GET_REQ_HEADER 88
#define DEL_REQ_HEADER 88
#define LIST_REQ_HEADER 8

#define GET_RESP_HEADER 8
#define PUT_RESP_HEADER 4
#define DEL_RESP_HEADER 4
#define LIST_RESP_HEADER 8

#define GET 0
#define PUT 1
#define DEL 2
#define LIST 3

struct files{
    char filename[FNAME_MAX];
    struct files *next;
};

#endif
