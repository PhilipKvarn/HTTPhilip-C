#include <stdio.h>
#include <stdlib.h>
#include "../include/html_server.h"

char* serve_html(const char* filename){

    FILE* file_ptr = fopen(filename,"r");
    if (!file_ptr) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file_ptr, 0, SEEK_END);
    long file_length = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    char* buffer = malloc(file_length + 1);
    if (!buffer) {
        perror("Error allocating memory");
        fclose(file_ptr);
        return NULL;
    }

    fread(buffer, 1, file_length, file_ptr);
    buffer[file_length] = '\0';

    fclose(file_ptr);
    return buffer;
}