#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../include/server.h"

#define PORT 8080

int main(){
    run_server(PORT);
    return 0;
}