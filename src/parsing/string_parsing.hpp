//
// Created by jayz on 17.01.20.
//

#ifndef PQUANTUM_PARSING_STRING_PARSING_HPP
#define PQUANTUM_PARSING_STRING_PARSING_HPP

#include <string>
#include "mathutils/tree_expression/arithmetic.hpp"

namespace PQuantum::parsing {
mathutils::tree_expression::arithmetic_tree parse_arithmetic_expression( const std::string &str );
}

#endif //PQUANTUM_PARSING_STRING_PARSING_HPP
