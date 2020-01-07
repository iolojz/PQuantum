cmake_minimum_required(VERSION 3.14)

add_library(model SHARED
		model_specification.cpp qft_json_parser.cpp)
target_link_libraries(model io mathutils error)
