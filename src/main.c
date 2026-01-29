#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "../include/html_server.h"
#include "../include/request_handler.h"

#define PORT 8080

char response[1000];

int main(){

    int server_fd, new_socket;

    struct sockaddr_in address;

    int opt = 1;

    char* html_content = serve_html("../src/html/index.html");
    

    int address_len = sizeof(address);
    char buffer[40000] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *) &address, address_len)) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 3);
    printf("Listening to port 8080....\n");
    while (true) {
        printf("Waiting for connection...\n");
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&address_len);
        if (new_socket < 0) {
            perror("Accept failed.\n");
            continue;
        }
        memset(buffer, 0, sizeof(buffer));
        printf("New Connection!\n");

        char method[32], path[32], http_version[32], ip[32];

        read(new_socket, buffer, 40000);
        char *str_input_buffer = buffer;
        const char *http_format = "%10s /%32s HTTP/%8s \r\nHost: %16s";                    //format string vulnerability, buffer skickas in av användaren
        sscanf(str_input_buffer, http_format,method,path,http_version,ip); //format string vulnerability, buffer skickas in av användaren
        
        printf("Method: %s\n",method);
        printf("Path: %s\n",path);
        printf("IP: %s\n",ip);
        printf("Version: %s\n\n",http_version);
        
        printf("%s\n",buffer);
        
        handle_request(new_socket);
    }
    close(server_fd);
    return 0;   
}