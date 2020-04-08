//
// Created by jayz on 02.04.20.
//

#ifndef PQUANTUM_CALCULATION_CALCULATION_HPP
#define PQUANTUM_CALCULATION_CALCULATION_HPP

namespace PQuantum {
namespace model {
class model_specification;
}

namespace calculation {
void calculate_beta_functions_with_wetterich_equation( const model::model_specification &model );
}
}

#endif // PQUANTUM_CALCULATION_CALCULATION_HPP
