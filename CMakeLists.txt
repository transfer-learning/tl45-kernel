project(TLOS)
cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE SRCS src/*.c src/**/*.c)
include_directories(./include)

add_executable(tlos ${SRCS})