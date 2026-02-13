cmake_minimum_required(VERSION 3.10)
project(httphilip_server C)

add_executable(server 
            src/html_server.c
            src/main.c 
            src/request_handler.c
            src/request_parser.c
            src/request_router.c
            src/server.c
            src/socket_handler.c
            )

target_include_directories(server PRIVATE include)