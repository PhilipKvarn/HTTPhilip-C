#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "../include/request_handler.h"
#include "../include/socket_handler.h"
#include "../include/server.h"

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
    
    listen(server_fd, 3);
    printf("Listening to port 8080....\n");
    while (true) {
        printf("Waiting for connection...\n");
        client_socket = accept_connection(server_fd, &address);
        if (client_socket < 0) {
            perror("Accept failed.\n");
            continue;
        }
        
        memset(buffer, 0, sizeof(buffer));
        printf("New Connection!\n");

        char method[32], path[32], http_version[32], ip[32];

        read(client_socket, buffer, 40000);
        char *str_input_buffer = buffer;
        const char *http_format = "%10s /%32s HTTP/%8s \r\nHost: %16s";              //format string vulnerability, buffer skickas in av användaren
        sscanf(str_input_buffer, http_format,method,path,http_version,ip); //format string vulnerability, buffer skickas in av användaren
        
        printf("Method: %s\r\nPath: %s\r\nIP: %s\r\nVersion: %s\r\n",method,path,ip,http_version);        
        printf("%s\n",buffer);
        
        handle_request(client_socket);
    }
    close(server_fd);
    return;   
}