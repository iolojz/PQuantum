cmake_minimum_required(VERSION 3.0)

find_package(BISON)
BISON_TARGET(math_parse math_parser.y ${CMAKE_CURRENT_BINARY_DIR}/math_parser.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(parsing SHARED
	${CMAKE_CURRENT_BINARY_DIR}/math_parser.cpp
	parse_math.cpp
	math_scanner.cpp
	math_error.cpp
	qft_json_parser.cpp math_to_lagrangian.cpp)

target_link_libraries(parsing model logging error)
