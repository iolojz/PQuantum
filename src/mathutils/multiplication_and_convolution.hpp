//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_MATHUTILS_MULTIPLICATION_AND_CONVOLUTION_HPP
#define PQUANTUM_MATHUTILS_MULTIPLICATION_AND_CONVOLUTION_HPP

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

#endif //PQUANTUM_MATHUTILS_MULTIPLICATION_AND_CONVOLUTION_HPP
