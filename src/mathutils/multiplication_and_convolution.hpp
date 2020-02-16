//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_MATHUTILS_MULTIPLICATION_AND_CONVOLUTION_HPP
#define PQUANTUM_MATHUTILS_MULTIPLICATION_AND_CONVOLUTION_HPP

#include "support/tree.hpp"

namespace PQuantum::mathutils {
struct multiplication_operator {};
struct fourier_convolution_operator {};
}

struct multiplication_and_convolution_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<
		multiplication_operator,
		fourier_convolution_operator
	>;
};

PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::multiplication_operator, 1 )
PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::fourier_convolution_operator, 1 )

#endif //PQUANTUM_MATHUTILS_MULTIPLICATION_AND_CONVOLUTION_HPP
