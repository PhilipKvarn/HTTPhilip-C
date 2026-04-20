#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

struct HttpRequest{
    char method[256];
    char path[256];
    char http_version[32];
    char host[256];
    char body[10000];
    char content_length[32];
};

int parse_http_request(const char* buffer, struct HttpRequest *request);

// Extract a form field value from request body
void extract_form_field(const char *body, const char *field_name, char *output, int output_size);

#endif