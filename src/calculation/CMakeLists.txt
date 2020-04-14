cmake_minimum_required(VERSION 3.0)

add_library(calculation SHARED
	field_derivative.cpp
	simplify_arithmetic.cpp
	zero_fields.cpp
	maximum_field_degree.cpp
	lagrangian_expansion.cpp
	map_fields.cpp
	wetterich/trace_terms_of_degrees.cpp
	wetterich/beta_functions.cpp)
target_link_libraries(calculation logging error)
