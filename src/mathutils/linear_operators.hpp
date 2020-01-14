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
struct product {
	static constexpr auto tree_node_arity = support::tree::runtime_arity;
};
struct sum {
	static constexpr auto tree_node_arity = support::tree::runtime_arity;
};

struct gamma_matrix {
	static constexpr auto tree_node_arity = 0;
	spacetime_index index;
};

struct spacetime_derivative {
	static constexpr auto tree_node_arity = 1;
	spacetime_index index;
};
struct dirac_operator {
	static constexpr auto tree_node_arity = 1;
};

struct multiplication_operator {
	static constexpr auto tree_node_arity = 1;
};
struct fourier_convolution_operator {
	static constexpr auto tree_node_arity = 1;
};

static constexpr auto linear_operators = boost::hana::tuple_t<
	product, sum, gamma_matrix, spacetime_derivative, dirac_operator, multiplication_operator, fourier_convolution_operator
>;
}

static constexpr auto linear_operator_types = linear_operators::linear_operators;
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::sum )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::product )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::gamma_matrix, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::spacetime_derivative, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::multiplication_operator )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::linear_operators::fourier_convolution_operator )

#endif //PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
