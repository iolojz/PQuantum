//
// Created by jayz on 10.02.20.
//

#ifndef PQUANTUM_PARSING_MATH_TYPES_HPP
#define PQUANTUM_PARSING_MATH_TYPES_HPP

#include "mathutils/number.hpp"

#include <string>

#include <boost/variant.hpp>

namespace PQuantum::parsing {
struct scanner_state {
	std::string::const_iterator current, end;
};

using number = mathutils::number;
using identifier = std::string;
using index = std::string;

using index_list = std::vector<index>;

struct index_spec {
	index_list lower, upper;
};

struct identifier_with_optional_indices {
	identifier name;
	index_spec indices;
};

struct function_call;

struct unary_minus;

struct binary_plus;
struct binary_minus;
struct binary_multiply;
struct binary_divide;
struct binary_power;

using arithmetic_expression = boost::variant<
	number,
	identifier_with_optional_indices,
	boost::recursive_wrapper<function_call>,
	boost::recursive_wrapper<unary_minus>,
	boost::recursive_wrapper<binary_plus>,
	boost::recursive_wrapper<binary_minus>,
	boost::recursive_wrapper<binary_multiply>,
	boost::recursive_wrapper<binary_divide>,
	boost::recursive_wrapper<binary_power>
>;

using argument_list = std::vector<arithmetic_expression>;

struct function_call {
	identifier name;
	argument_list arguments;
};

struct unary_minus {
	arithmetic_expression argument;
};

struct binary_plus {
	std::array<arithmetic_expression, 2> arguments;
};

struct binary_minus {
	std::array<arithmetic_expression, 2> arguments;
};

struct binary_multiply {
	std::array<arithmetic_expression, 2> arguments;
};

struct binary_divide {
	std::array<arithmetic_expression, 2> arguments;
};

struct binary_power {
	std::array<arithmetic_expression, 2> arguments;
};
}

#endif //PQUANTUM_PARSING_MATH_TYPES_HPP
