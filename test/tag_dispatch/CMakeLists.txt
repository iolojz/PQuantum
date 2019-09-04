cmake_minimum_required(VERSION 3.14)

add_executable(tag_dispatch tag_dispatch.cpp)
target_link_libraries(tag_dispatch)
add_test(tag_dispatch tag_dispatch)
