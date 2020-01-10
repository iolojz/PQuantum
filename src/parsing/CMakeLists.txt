cmake_minimum_required(VERSION 3.0)

add_library(parsing SHARED
        qft_json_parser.cpp)

target_link_libraries(parsing model logging)
