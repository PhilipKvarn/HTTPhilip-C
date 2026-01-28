cmake_minimum_required(VERSION 3.10)
project(httphilip_server C)

add_executable(server src/main.c)

target_include_directories(server PRIVATE include)