#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/html_server.h"
#include "../include/request_parser.h"
#include "../include/database.h"

//
//  init new route with
//  void handle_get_index(int socket, struct HttpRequest *request)
//

void handle_get_index(int socket, struct HttpRequest *request){
    // Fetch all events from database
    EventList list = get_all_events();
    
    // Build HTML response
    char *response = malloc(50000);
    if (!response) {
        const char* error_response = 
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>500 Internal Server Error</h1>";
        write(socket, error_response, strlen(error_response));
        close(socket);
        return;
    }
    
    // Start building HTML
    int offset = 0;
    offset += sprintf(response + offset, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "  <title>Events</title>\n"
        "  <style>\n"
        "    body { font-family: Arial, sans-serif; margin: 20px; }\n"
        "    h1 { color: #333; }\n"
        "    .create-link { margin-bottom: 20px; }\n"
        "    .create-link a { background-color: #4CAF50; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; }\n"
        "    .create-link a:hover { background-color: #45a049; }\n"
        "    table { border-collapse: collapse; width: 100%%; margin-top: 20px; }\n"
        "    th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }\n"
        "    th { background-color: #4CAF50; color: white; }\n"
        "    tr:nth-child(even) { background-color: #f2f2f2; }\n"
        "    .no-events { color: #666; margin-top: 20px; }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <h1>Events</h1>\n"
        "  <div class=\"create-link\">\n"
        "    <a href=\"/Create\">+ Create New Event</a>\n"
        "  </div>\n");
    
    if (list.count == 0) {
        offset += sprintf(response + offset, "  <p class=\"no-events\">No events yet. <a href=\"/Create\">Create one!</a></p>\n");
    } else {
        offset += sprintf(response + offset,
            "  <table>\n"
            "    <tr>\n"
            "      <th>Poster</th>\n"
            "      <th>Title</th>\n"
            "      <th>Description</th>\n"
            "      <th>Created</th>\n"
            "      <th>Event Date</th>\n"
            "    </tr>\n");
        
        // Add event rows
        for (int i = 0; i < list.count; i++) {
            Event *e = &list.events[i];
            offset += sprintf(response + offset,
                "    <tr>\n"
                "      <td>%s</td>\n"
                "      <td>%s</td>\n"
                "      <td>%.100s...</td>\n"
                "      <td>%s</td>\n"
                "      <td>%s</td>\n"
                "    </tr>\n",
                e->poster_name,
                e->title,
                e->description,
                e->created_date,
                e->event_date);
        }
        
        offset += sprintf(response + offset, "  </table>\n");
    }
    
    offset += sprintf(response + offset,
        "</body>\n"
        "</html>\n");
    
    write(socket, response, strlen(response));
    free(response);
    free_events(list);
    close(socket);
}

void handle_get_create(int socket, struct HttpRequest *request){
    char* html_content;

    html_content = serve_html("./public/create.html");

    if (html_content) {
        const char* content_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(socket, content_header, strlen(content_header));
        write(socket, html_content, strlen(html_content));
    } else {
        const char* not_found_response = 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>404 Not Found</h1>"; 
        write(socket, not_found_response, strlen(not_found_response));
    }

    free(html_content);
    close(socket);
}

void handle_post_create(int socket, struct HttpRequest *request){
    // Extract form fields from request body
    char poster_name[256] = {0};
    char title[128] = {0};
    char description[2048] = {0};
    char event_date[32] = {0};
    
    extract_form_field(request->body, "poster_name", poster_name, sizeof(poster_name));
    extract_form_field(request->body, "title", title, sizeof(title));
    extract_form_field(request->body, "description", description, sizeof(description));
    extract_form_field(request->body, "event_date", event_date, sizeof(event_date));
    
    // Validate fields
    if (strlen(poster_name) == 0 || strlen(title) == 0 || 
        strlen(description) == 0 || strlen(event_date) == 0) {
        const char* error_response = 
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>400 Bad Request</h1>"
            "<p>All fields are required.</p>";
        write(socket, error_response, strlen(error_response));
        close(socket);
        return;
    }
    
    if (strlen(title) > 128) {
        const char* error_response = 
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>400 Bad Request</h1>"
            "<p>Title must be 128 characters or less.</p>";
        write(socket, error_response, strlen(error_response));
        close(socket);
        return;
    }
    
    if (strlen(description) > 2048) {
        const char* error_response = 
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>400 Bad Request</h1>"
            "<p>Description must be 2048 characters or less.</p>";
        write(socket, error_response, strlen(error_response));
        close(socket);
        return;
    }
    
    // Insert event into database
    if (insert_event(poster_name, title, description, event_date) == 0) {
        const char* error_response = 
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>500 Internal Server Error</h1>"
            "<p>Failed to create event.</p>";
        write(socket, error_response, strlen(error_response));
        close(socket);
        return;
    }
    
    // Success - redirect to index
    const char* redirect_response = 
        "HTTP/1.1 302 Found\r\n"
        "Location: /\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
    write(socket, redirect_response, strlen(redirect_response));
    close(socket);
}