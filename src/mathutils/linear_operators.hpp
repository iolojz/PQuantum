//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
#define PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include "spacetime_index.hpp"

namespace PQuantum::mathutils
{
namespace linear_operators
{
struct sum
{
};
struct product
{
};

struct gamma_matrix
{
	spacetime_index index;
};
struct sigma_matrix
{
	spacetime_index index1, index2;
};

struct spacetime_derivative
{
	spacetime_index index;
};
struct dirac_operator
{
};

struct multiplication_operator
{
};
struct fourier_convolution_operator
{
};

using linear_operators = std::tuple<sum, product, gamma_matrix, sigma_matrix, spacetime_derivative, dirac_operator, multiplication_operator, fourier_convolution_operator>;
}

using linear_operators::linear_operators;
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::sum )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::product )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::gamma_matrix, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::sigma_matrix, index1, index2 )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::spacetime_derivative, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::multiplication_operator )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::fourier_convolution_operator )

#endif //PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
