#include "../include/request_router.h"
#include <stdio.h>
#include <string.h>

#define MAX_ROUTES 50

static struct Route routes[MAX_ROUTES];
static int route_count = 0;

void register_route(const char *method, const char *path, RouteHandler handler){

    if (route_count >= MAX_ROUTES) {
        printf("Error: exceeded max routes");
        return;
    }

    strncpy(routes[route_count].method, method, sizeof(routes[route_count].method)-1);
    strncpy(routes[route_count].path, path, sizeof(routes[route_count].method)-1);
    routes[route_count].handler = handler;

    route_count++;
}

RouteHandler find_route(const char *method, const char *path){
    for (int i = 0; i < route_count; i++) {
        if (
            strcmp(routes[route_count].method, method) == 0 && 
            strcmp(routes[route_count].path,path) == 0  
        ) {
            return routes[route_count].handler;
        }   
    }
    return NULL;
}

