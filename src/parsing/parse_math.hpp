//
// Created by jayz on 17.01.20.
//

#ifndef PQUANTUM_PARSING_STRING_PARSING_HPP
#define PQUANTUM_PARSING_STRING_PARSING_HPP

#include "bison_types.hpp"

namespace PQuantum::parsing {
math_tree_node parse_math( const std::string &str );
}

#endif //PQUANTUM_PARSING_STRING_PARSING_HPP
