cmake_minimum_required(VERSION 3.0)

add_library(model SHARED
		model_specification.cpp qft_json_parser.cpp)
target_link_libraries(model io mathutils error)
