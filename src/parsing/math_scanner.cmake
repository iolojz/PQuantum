//
// Created by jayz on 11.02.20.
//

#ifndef PQUANTUM_PARSING_MATH_SCANNER_HPP
#define PQUANTUM_PARSING_MATH_SCANNER_HPP

#include "bison_types.hpp"
#include "${CMAKE_CURRENT_BINARY_DIR}/math_parse.hpp"

namespace PQuantum::parsing::bison {
parser::symbol_type yylex( scanner_state &state );
}

#endif //PQUANTUM_PARSING_MATH_SCANNER_HPP
