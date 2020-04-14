//
// Created by jayz on 14.04.20.
//

#ifndef PQUANTUM_CALCULATION_BETA_FUNCTION_HPP
#define PQUANTUM_CALCULATION_BETA_FUNCTION_HPP

#include "../model/lagrangian.hpp"

namespace PQuantum::calculation {
using beta_function = std::pair<model::indexed_parameter, model::lagrangian_tree>;
}

#endif //PQUANTUM_CALCULATION_BETA_FUNCTION_HPP
