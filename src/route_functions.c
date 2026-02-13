#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/html_server.h"

//
//  init new route with
//  void handle_get_index(int socket, struct HttpRequest *request)
//

void handle_get_index(int socket, struct HttpRequest *request){
    char* html_content;

    html_content = serve_html("../public/index.html");

    if (html_content) {
        const char* content_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(socket, content_header, strlen(content_header));
        write(socket, html_content, strlen(html_content));
    } else {
        const char* not_found_response = 
        "HTTP/1.1 4O4 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>404 Not Found</h1>"; 
        write(socket, not_found_response, strlen(not_found_response));
    }

    free(html_content);
    close(socket);
}

void handle_get_create(int socket, struct HttpRequest *request){
    char* html_content;

    html_content = serve_html("../public/create.html");

    if (html_content) {
        const char* content_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(socket, content_header, strlen(content_header));
        write(socket, html_content, strlen(html_content));
    } else {
        const char* not_found_response = 
        "HTTP/1.1 4O4 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>404 Not Found</h1>"; 
        write(socket, not_found_response, strlen(not_found_response));
    }

    free(html_content);
    close(socket);
}