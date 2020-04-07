//
// Created by jayz on 07.04.20.
//

#ifndef PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
#define PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP

#include "spacetime_index.hpp"

namespace PQuantum::mathutils {
struct gamma_matrix {
	spacetime_index index;
	
	constexpr bool operator==( const gamma_matrix &other ) const {
		return index == other.index;
	}
};

struct spacetime_derivative {
	spacetime_index index;
	
	constexpr bool operator==( const spacetime_derivative &other ) const {
		return index == other.index;
	}
};

struct dirac_operator {
	constexpr bool operator==( const dirac_operator & ) const { return true; }
};

struct field_multiplication_operator {
	constexpr bool operator==( const field_multiplication_operator & ) const { return true; }
};

struct fourier_field_convolution_operator {
	constexpr bool operator==( const fourier_field_convolution_operator & ) const { return true; }
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const gamma_matrix &gm ) {
	return os << "\\gamma" << gm.index;
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const spacetime_derivative &sd ) {
	return os << "\\SpacetimeDerivative" << sd.index;
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const dirac_operator & ) {
	return os << "\\DiracOperator";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const field_multiplication_operator & ) {
	return os << "M";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const fourier_field_convolution_operator & ) {
	return os << "\\tilde{C}";
}
}

#endif //PQUANTUM_MATHUTILS_LINEAR_OPERATORS_HPP
