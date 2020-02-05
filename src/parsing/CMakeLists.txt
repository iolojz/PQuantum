cmake_minimum_required(VERSION 3.0)

add_library(parsing SHARED
		parsing.cpp)

target_link_libraries(parsing logging error)
