cmake_minimum_required(VERSION 3.14)

add_library(io SHARED
	program_options_handler.cpp
	logging.cpp qft_json_parser.cpp math_parser.cpp math_parser.hpp)

target_link_libraries(io Boost::log Boost::program_options)
