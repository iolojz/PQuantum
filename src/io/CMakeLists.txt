cmake_minimum_required(VERSION 3.0)

add_library(io SHARED
		program_options_handler.cpp
		logging.cpp math_parser.cpp)

target_link_libraries(io Boost::log Boost::program_options)
