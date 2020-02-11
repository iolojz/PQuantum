cmake_minimum_required(VERSION 3.0)

find_package(BISON)
BISON_TARGET(math_parse math_parse.y ${CMAKE_CURRENT_BINARY_DIR}/math_parse.cpp)

configure_file(bison_types.cmake ${${PROJECT_NAME}_BINARY_DIR}/include/parsing/bison_types.hpp)
configure_file(math_scanner.cmake ${${PROJECT_NAME}_BINARY_DIR}/include/parsing/math_scanner.hpp)
configure_file(parsing.cmake ${CMAKE_CURRENT_BINARY_DIR}/parsing.cpp)

add_library(parsing SHARED
	${CMAKE_CURRENT_BINARY_DIR}/math_parse.cpp
	${CMAKE_CURRENT_BINARY_DIR}/parsing.cpp
	math_scanner.cpp)

target_link_libraries(parsing logging error)
