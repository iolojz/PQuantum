cmake_minimum_required(VERSION 3.0)

find_package(BISON)
BISON_TARGET(math_parse math_parse.y ${CMAKE_CURRENT_BINARY_DIR}/math_parse.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(parsing SHARED
	${CMAKE_CURRENT_BINARY_DIR}/math_parse.cpp
	parsing.cpp
	math_scanner.cpp
	math_error.cpp)

target_link_libraries(parsing logging error)
