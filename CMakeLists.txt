cmake_minimum_required(VERSION 3.10)

project(tieto_zadanie)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_C_COMPILER_ID MATCHES GNU)
    set(CMAKE_C_FLAGS "-Wall -pedantic -Wextra")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Werror -O3")
endif()

if (CMAKE_C_COMPILER_ID MATCHES Clang)
    set(CMAKE_C_FLAGS "-Weverything -pedantic -Wno-disabled-macro-expansion -Wno-padded -Wno-declaration-after-statement")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Werror -O3")
endif()


include_directories(src)
include_directories(inc)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_library(TestedFiles 
            src/proc_parser.c
            src/circular_buffer.c)