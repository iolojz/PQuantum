//
// Created by jayz on 17.01.20.
//

#ifndef PQUANTUM_PARSING_STRING_PARSING_HPP
#define PQUANTUM_PARSING_STRING_PARSING_HPP

#include <string>

#include "instantiations.hpp"

namespace PQuantum::parsing {
typename instantiations::arithmetic_tree_tag::type parse_arithmetic_expression( const std::string &str );
}

#endif //PQUANTUM_PARSING_STRING_PARSING_HPP
