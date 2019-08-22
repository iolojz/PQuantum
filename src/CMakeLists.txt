cmake_minimum_required(VERSION 3.14)

add_subdirectory(io)
add_subdirectory(error)
add_subdirectory(model)
add_subdirectory(mathutils)

add_executable(pquantum main.cpp)
target_link_libraries(pquantum io error model)
