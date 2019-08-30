cmake_minimum_required(VERSION 3.14)

add_executable(clifford clifford.cpp)
target_link_libraries(clifford mathutils io error model)
add_test(clifford clifford)
