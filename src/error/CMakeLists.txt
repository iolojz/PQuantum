cmake_minimum_required(VERSION 3.14)

add_library(error SHARED
	error.cpp)
target_link_libraries(error io)
