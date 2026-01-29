#include "../include/socket_handler.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>

int create_server_socket(int port){
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address))) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int accept_connection(int server_fd, struct sockaddr_in *address){
    socklen_t address_len = sizeof(*address);
    int acc = accept(server_fd, (struct sockaddr *)address, &address_len);
    return acc;
}