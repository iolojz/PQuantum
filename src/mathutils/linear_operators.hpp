//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
#define PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include "spacetime_index.hpp"
#include "support/tree.hpp"

namespace PQuantum::mathutils
{
namespace linear_operators
{
struct gamma_matrix {
	spacetime_index index;
};

struct spacetime_derivative {
	spacetime_index index;
};
struct dirac_operator {};

struct multiplication_operator {};
struct fourier_convolution_operator {};
}
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::linear_operators::gamma_matrix, 1)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::linear_operators::spacetime_derivative, 1)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::linear_operators::dirac_operator, 0)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::linear_operators::multiplication_operator, 1)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::linear_operators::fourier_convolution_operator, 1)

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::gamma_matrix, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::spacetime_derivative, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::dirac_operator )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::multiplication_operator )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::fourier_convolution_operator )

#endif //PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
