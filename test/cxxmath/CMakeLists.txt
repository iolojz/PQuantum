cmake_minimum_required(VERSION 3.14)

add_executable(bool bool.cpp)
target_link_libraries(bool)
add_test(bool bool)

add_executable(int int.cpp)
target_link_libraries(int)
add_test(int int)

add_executable(std_vector std_vector.cpp)
target_link_libraries(std_vector)
add_test(std_vector std_vector)

add_executable(std_pair std_pair.cpp)
target_link_libraries(std_pair)
add_test(std_pair std_pair)

add_executable(product_set product_set.cpp)
target_link_libraries(product_set)
add_test(product_set product_set)

add_executable(product_monoid product_monoid.cpp)
target_link_libraries(product_monoid)
add_test(product_monoid product_monoid)

add_executable(polynomial polynomial.cpp)
target_link_libraries(polynomial)
add_test(polynomial polynomial)
