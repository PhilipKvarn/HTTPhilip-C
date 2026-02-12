#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/request_handler.h"
#include "../include/request_parser.h"
#include "../include/html_server.h"

#define BUFFER_SIZE  1024

void handle_request(int new_socket, struct HttpRequest *request){

    char* html_content;

    html_content = serve_html("../public/index.html");

    if (html_content) {
        const char* content_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(new_socket, content_header, strlen(content_header));
        write(new_socket, html_content, strlen(html_content));
    } else {
        const char* not_found_response = 
        "HTTP/1.1 4O4 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>404 Not Found</h1>"; 
        write(new_socket, not_found_response, strlen(not_found_response));
    }

    free(html_content);
    close(new_socket);
}
