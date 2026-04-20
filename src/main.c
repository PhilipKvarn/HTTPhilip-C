#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/server.h"
#include "../include/database.h"

#define PORT 8080

int main(){
    if (init_database() == 0) {
        fprintf(stderr, "Failed to initialize database\n");
        return 1;
    }
    run_server(PORT);
    close_database();
    return 0;
}