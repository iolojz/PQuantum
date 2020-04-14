//
// Created by jayz on 02.04.20.
//

#ifndef PQUANTUM_CALCULATION_CALCULATION_HPP
#define PQUANTUM_CALCULATION_CALCULATION_HPP

#include "beta_function.hpp"

namespace PQuantum {
namespace model {
class model_specification;
}

namespace calculation::wetterich {
void calculate_beta_functions( const model::model_specification &model );
}
}

#endif // PQUANTUM_CALCULATION_CALCULATION_HPP
