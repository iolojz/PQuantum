cmake_minimum_required(VERSION 3.0)

find_package(BISON)
BISON_TARGET(math_parse math_parse.y ${CMAKE_CURRENT_BINARY_DIR}/math_parse.cpp)

configure_file(config.cmake ${${PROJECT_NAME}_BINARY_DIR}/include/parsing/config.hpp)

add_library(parsing SHARED
	${CMAKE_CURRENT_BINARY_DIR}/math_parse.cpp math_scanner.cpp)

target_link_libraries(parsing logging error)
