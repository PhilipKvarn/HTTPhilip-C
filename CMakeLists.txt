cmake_minimum_required(VERSION 3.10)
project(httphilip_server C)

add_executable(server 
            src/main.c 
            src/html_server.c 
            src/request_handler.c
            src/server.c
            src/socket_handler.c
            )

target_include_directories(server PRIVATE include)