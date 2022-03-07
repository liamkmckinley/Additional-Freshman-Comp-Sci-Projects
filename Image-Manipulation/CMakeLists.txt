cmake_minimum_required(VERSION 3.15)
project(IP_FIVERR C)

set(CMAKE_C_STANDARD 99)
set(SOURCES ppm_io.h main.c ppm_io.c )
add_executable(IP_FIVERR ${SOURCES})
target_link_libraries(IP_FIVERR m)
