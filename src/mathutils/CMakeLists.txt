cmake_minimum_required(VERSION 3.0)

add_library(mathutils SHARED
        number.cpp atom.hpp)
target_link_libraries(mathutils logging)
