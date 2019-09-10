cmake_minimum_required(VERSION 3.14)

add_executable(bool bool.cpp)
target_link_libraries(bool)
add_test(bool bool)

add_executable(int int.cpp)
target_link_libraries(int)
add_test(int int)

add_executable(free_monomial free_monomial.cpp)
target_link_libraries(free_monomial)
add_test(free_monomial free_monomial)

add_executable(free_polynomial free_polynomial.cpp)
target_link_libraries(free_polynomial)
add_test(free_polynomial free_polynomial)
