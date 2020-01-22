cmake_minimum_required(VERSION 3.15)
project(Pixmap_multi_thread C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_definitions(${GTK_CFLAGS_OTHER})

add_executable(Pixmap_multi_thread main.c)

target_link_libraries(Pixmap_multi_thread LINK_PUBLIC sqlite3)
target_link_libraries(Pixmap_multi_thread LINK_PUBLIC ${GTK_LIBRARIES})