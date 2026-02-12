#ifndef HTML_REQUESTHANDLER_H
#define HTML_REQUESTHANDLER_H
#include "./request_parser.h"

void handle_request(int new_socket, struct HttpRequest *request);

#endif