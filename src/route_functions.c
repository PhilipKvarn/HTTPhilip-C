#include "../include/html_server.h"

//
//  init new route with
//  void handle_get_index(int socket, struct HttpRequest *request)
//

void handle_get_index(int socket, struct HttpRequest *request){
    char* html_content;
    html_content = serve_html("../public/index.html");
}