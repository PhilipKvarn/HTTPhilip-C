#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "../include/request_handler.h"
#include "../include/request_parser.h"
#include "../include/request_router.h"
#include "../include/socket_handler.h"
#include "../include/server.h"
#include "route_functions.c"

void run_server(int server_port){

    int client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int address_len = sizeof(address);
    char buffer[40000] = {0};
    

    int server_fd = create_server_socket(server_port);
    if (server_fd < 0) {
        perror("Failed to create server socket");
        return;
    }

    register_route("GET", "/", handle_get_index);
        
    
    listen(server_fd, 3);
    printf("Listening to port 8080....\n");
    while (true) {
        
        printf("Waiting for connection...\n");
        client_socket = accept_connection(server_fd, &address);
        if (client_socket < 0) {
            perror("Accept failed.\n");
            continue;
        }
        
        struct HttpRequest request;

        
        memset(buffer, 0, sizeof(buffer));
        printf("New Connection!\n");

        read(client_socket, buffer, 40000);
        char *str_input_buffer = buffer;

        
        if (parse_http_request(str_input_buffer, &request) == 0) {
            printf("Method: %s\r\nPath: %s\r\nIP: %s\r\nVersion: %s\r\n",request.method,request.path,request.host,request.http_version);        
            printf("%s\n",buffer);
            handle_request(client_socket , &request);
        } else {
            printf("failed request parsing\n");
            close(client_socket);
        } 
    }
    close(server_fd);
    return;   
}