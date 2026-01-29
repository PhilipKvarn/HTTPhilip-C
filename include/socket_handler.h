#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include <netinet/in.h>

int create_server_socket(int port);
int accept_connection(int server_fd, struct sockaddr_in *address);

#endif