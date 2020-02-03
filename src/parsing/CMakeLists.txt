cmake_minimum_required(VERSION 3.0)

add_library(parsing SHARED
    qft_json_parser.cpp
	string_parsing.cpp
	type_rules/product.cpp
	type_rules/quotient.cpp
	type_rules/sum.cpp
	type_rules/difference.cpp type_rules/identifier.cpp type_rules/identifier.hpp)

target_link_libraries(parsing model logging)
