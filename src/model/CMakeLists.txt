cmake_minimum_required(VERSION 3.0)

add_library(model SHARED
        model_specification.cpp)
target_link_libraries(model error logging calculation)
