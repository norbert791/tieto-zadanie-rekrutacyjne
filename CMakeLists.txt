cmake_minimum_required(VERSION 3.10)

project(tieto_zadanie)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(src)
include_directories(inc)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)