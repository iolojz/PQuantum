cmake_minimum_required(VERSION 3.0)

add_executable(parse_math parse_math.cpp)
target_link_libraries(parse_math parsing)
add_test(parse_math parse_math)

add_executable(parse_qed parse_qed.cpp)
target_link_libraries(parse_qed parsing mathutils logging error model)
add_test(parse_qed parse_qed)
