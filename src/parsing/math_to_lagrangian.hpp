//
// Created by jayz on 13.02.20.
//

#ifndef PQUANTUM_PARSING_MATH_TO_LAGRANGIAN_HPP
#define PQUANTUM_PARSING_MATH_TO_LAGRANGIAN_HPP

#include "bison_types.hpp"
#include "qft_parsing_context.hpp"

#include "model/lagrangian.hpp"

namespace PQuantum::parsing {
model::lagrangian_tree math_to_lagrangian( math_tree &&mtree, qft_parsing_context &&qft_context );
}

#endif //PQUANTUM_PARSING_MATH_TO_LAGRANGIAN_HPP
