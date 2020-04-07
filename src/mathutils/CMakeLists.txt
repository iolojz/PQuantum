cmake_minimum_required(VERSION 3.0)

add_library(mathutils SHARED
        number.cpp)
target_link_libraries(mathutils logging)
