cmake_minimum_required(VERSION 3.0)

add_library(calculation SHARED
	beta_functions_wetterich.cpp lagrangian_derivative.cpp)
