#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/request_parser.h"

// Helper function to URL decode a string
static void url_decode(char *src, char *dst, int dst_size) {
    if (!src || !dst || dst_size <= 0) return;
    
    int src_len = strlen(src);
    int dst_idx = 0;
    
    for (int i = 0; i < src_len && dst_idx < dst_size - 1; i++) {
        if (src[i] == '%' && i + 2 < src_len) {
            // Parse hex-encoded character
            int hex_val;
            if (sscanf(&src[i + 1], "%2x", &hex_val) == 1) {
                dst[dst_idx++] = (char)hex_val;
                i += 2;
            } else {
                dst[dst_idx++] = src[i];
            }
        } else if (src[i] == '+') {
            // Plus sign represents space in form data
            dst[dst_idx++] = ' ';
        } else {
            dst[dst_idx++] = src[i];
        }
    }
    dst[dst_idx] = '\0';
}

// Helper function to extract a form field value from body
void extract_form_field(const char *body, const char *field_name, char *output, int output_size) {
    if (!body || !field_name || !output || output_size <= 0) {
        if (output && output_size > 0) output[0] = '\0';
        return;
    }
    
    output[0] = '\0';
    
    // Find the field name in the body
    char search_pattern[512];
    snprintf(search_pattern, sizeof(search_pattern), "%s=", field_name);
    
    const char *field_start = strstr(body, search_pattern);
    if (!field_start) {
        return;
    }
    
    // Move past the "fieldname=" part
    const char *value_start = field_start + strlen(search_pattern);
    
    // Find the end of the field value (next & or end of string)
    const char *value_end = strchr(value_start, '&');
    if (!value_end) {
        value_end = value_start + strlen(value_start);
    }
    
    // Extract and decode the value
    int value_len = value_end - value_start;
    if (value_len > 0) {
        char temp[10000];
        strncpy(temp, value_start, value_len);
        temp[value_len] = '\0';
        url_decode(temp, output, output_size);
    }
}

int parse_http_request(const char* buffer, struct HttpRequest *request){
    if (!buffer || !request) {
        return EXIT_FAILURE;
    }

    memset(request, 0, sizeof(struct HttpRequest));

    // Parse request line: METHOD PATH HTTP/VERSION
    const char *http_format = "%31s %255s HTTP/%31s";
    int is_parsed = sscanf(buffer, http_format,
        request->method,
        request->path,
        request->http_version
    );

    if (is_parsed != 3) {
        return EXIT_FAILURE;
    }

    // Extract Host header
    const char *host_start = strstr(buffer, "Host: ");
    if (host_start) {
        host_start += 6; // Skip "Host: "
        sscanf(host_start, "%255s", request->host);
    }

    // Extract Content-Length header
    const char *content_length_start = strstr(buffer, "Content-Length: ");
    if (content_length_start) {
        content_length_start += 16; // Skip "Content-Length: "
        sscanf(content_length_start, "%31s", request->content_length);
    }

    // Extract body (everything after the blank line)
    const char *body_start = strstr(buffer, "\r\n\r\n");
    if (body_start) {
        body_start += 4; // Skip the \r\n\r\n
        strncpy(request->body, body_start, sizeof(request->body) - 1);
        request->body[sizeof(request->body) - 1] = '\0';
    }

    return EXIT_SUCCESS;
}
