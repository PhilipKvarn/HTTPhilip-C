#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include "request_parser.h"

typedef void (*RouteHandler)(int new_socket, struct HttpRequest *request);

struct Route{
    char method[8];
    char path[256];
    RouteHandler handler;
};

void init_routes(void);

void register_route(const char* method, const char* path, RouteHandler handler);

RouteHandler find_route(const char* method,const char* path);

#endif