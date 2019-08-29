cmake_minimum_required(VERSION 3.14)

add_library(mathutils SHARED
        number.cpp manifold_specification.cpp)
target_link_libraries(mathutils io)
