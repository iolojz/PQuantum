//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_LAGRANGIAN_HPP
#define PQUANTUM_LAGRANGIAN_HPP

#include "mathutils/expressions.hpp"
#include "mathutils/linear_operators.hpp"
#include "mathutils/free_polynomial.hpp"

#include "classical_field.hpp"

#include <boost/property_tree/ptree_fwd.hpp>
#include <variant>

namespace PQuantum
{
namespace model
{
using lagrangian_symbol = std::variant<mathutils::linear_operator, classical_field_id>;

using lagrangian = mathutils::free_polynomial<lagrangian_symbol, mathutils::polynomial_expression //< coefficients
>;
}
}

#endif //PQUANTUM_LAGRANGIAN_HPP
