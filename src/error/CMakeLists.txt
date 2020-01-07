cmake_minimum_required(VERSION 3.0)

add_library(error SHARED
	error.cpp)
target_link_libraries(error io)
