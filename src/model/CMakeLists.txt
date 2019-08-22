cmake_minimum_required(VERSION 3.14)

add_library(model SHARED
	model_specification.cpp)
target_link_libraries(model io mathutils)
