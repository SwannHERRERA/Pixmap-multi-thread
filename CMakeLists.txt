cmake_minimum_required(VERSION 3.15)
project(Pixmap_multi_thread C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Pixmap_multi_thread
        main.c)
