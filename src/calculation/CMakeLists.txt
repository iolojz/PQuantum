cmake_minimum_required(VERSION 3.0)

add_library(calculation SHARED
	beta_functions_wetterich.cpp
	lagrangian_derivative.cpp
	simplify_arithmetic.cpp
	zero_fields.cpp
	maximum_field_degree.cpp)
target_link_libraries(calculation logging error)
