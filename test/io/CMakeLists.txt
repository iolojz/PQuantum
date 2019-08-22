cmake_minimum_required(VERSION 3.14)

add_executable(parse_qed parse_qed.cpp)
target_link_libraries(parse_qed mathutils io error model)
add_test(parse_qed parse_qed)
