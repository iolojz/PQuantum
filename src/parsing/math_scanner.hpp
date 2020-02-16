//
// Created by jayz on 11.02.20.
//

#ifndef PQUANTUM_PARSING_MATH_SCANNER_HPP
#define PQUANTUM_PARSING_MATH_SCANNER_HPP

#include "bison_types.hpp"

namespace PQuantum::parsing {
struct scanner_state {
	std::string::const_iterator current;
	const std::string::const_iterator end;
};

struct input_location {
	std::decay_t<decltype( std::declval<scanner_state>().current )> begin, end;
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const input_location &loc ) {
	return os << std::string{loc.begin, loc.end};
}
}

#include "math_parser.hpp"

namespace PQuantum::parsing::bison {
parser::symbol_type yylex( scanner_state &state );
}

#endif //PQUANTUM_PARSING_MATH_SCANNER_HPP
