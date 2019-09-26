cmake_minimum_required(VERSION 3.14)

add_executable(polynomial polynomial.cpp)
target_link_libraries(polynomial mathutils io error model)
add_test(polynomial polynomial)
