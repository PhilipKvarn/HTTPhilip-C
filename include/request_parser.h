#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct HttpRequest{
    char method[256];
    char path[256];
    char http_version[32];
    char host[256];
};

int parse_http_request(const char* buffer, struct HttpRequest *request);


#endif