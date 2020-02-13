//
// Created by jayz on 13.02.20.
//

#ifndef PQUANTUM_PARSING_MATH_TO_LAGRANGIAN_HPP
#define PQUANTUM_PARSING_MATH_TO_LAGRANGIAN_HPP

#include "bison_types.hpp"
#include "qft_parsing_context.hpp"

#include "model/lagrangian.hpp"

namespace PQuantum::parsing {
model::lagrangian_node math_to_lagrangian( math_tree_node &&math_node, qft_parsing_context &&qft_context );
}

#endif //PQUANTUM_PARSING_MATH_TO_LAGRANGIAN_HPP
