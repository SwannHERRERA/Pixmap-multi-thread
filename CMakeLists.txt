cmake_minimum_required(VERSION 3.1)
project(Pixmap_multi_thread C)

set(CMAKE_C_STANDARD 11)

add_executable(Pixmap_multi_thread main.c ppm_image.c)


TARGET_LINK_LIBRARIES(Pixmap_multi_thread pthread)