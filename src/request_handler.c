#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/request_handler.h"
#include "../include/html_server.h"

#define BUFFER_SIZE  1024

void handle_request(int new_socket){
    char buffer[BUFFER_SIZE] = {0};

    char* html_content = serve_html("../src/html/index.html");
    if (html_content) {
        write(new_socket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n", 48);
        write(new_socket, html_content, strlen(html_content));
    } else {
        const char* not_found_response = "HTTP/1.1 4O4 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>"; 
        write(new_socket, not_found_response, strlen(not_found_response));
    }

    free(html_content);
    close(new_socket);
}