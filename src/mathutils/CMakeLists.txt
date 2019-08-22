cmake_minimum_required(VERSION 3.14)

add_library(mathutils SHARED
	free_polynomial.hpp number.cpp number.hpp manifold_specification.cpp)
target_link_libraries(mathutils io)
