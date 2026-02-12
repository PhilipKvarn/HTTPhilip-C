#include "../include/request_parser.h"

int parse_http_request(const char* buffer, struct HttpRequest *request){
    if (!buffer || !request) {
        return EXIT_FAILURE;
    }

    memset(request, 0, sizeof(struct HttpRequest));

    const char *http_format = "%31s %255s HTTP/%31s \r\nHost: %255s";
    int is_parsed = sscanf(buffer, http_format,
        request->method,
        request->path,
        request->http_version,
        request->host
    );

    if (is_parsed == 4) {
        return EXIT_SUCCESS;
    } else {
        printf("exitfailure 2");
        return EXIT_FAILURE;
    }

}
