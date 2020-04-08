//
// Created by jayz on 08.04.20.
//

#ifndef PQUANTUM_CALCULATION_LAGRANGIAN_POLYNOMIAL_HPP
#define PQUANTUM_CALCULATION_LAGRANGIAN_POLYNOMIAL_HPP

#include <cxxmath/cxxmath.hpp>

#include "../model/lagrangian.hpp"

namespace PQuantum::cxxmath_compatibility {
struct less_model_parameters {
	static constexpr apply( const model::indexed_parameter &p1, const model::indexed_parameter &p2 ) {
		if( p1.id < p2.id )
			return true;
		if( p1.id > p2.id )
			return false;
		
		return cxxmath::less( p1.indices, p2.indices );
	}
};

struct less_lagrangian_polynomial_symbol {
	static constexpr apply(
		const calculation::lagrangian_polynomial_symbol &s1,
		const calculation::lagrangian_polynomial_symbol &s2
	) {
		if( s1.index() < s2.index() )
			return true;
		if( s1.index() > s2.index() )
			return false;
		
		return std::visit(
			[] ( const auto &v1, const auto &v2 ) {
				if constexpr( std::is_same_v<std::decay_t<decltype(v1)>, std::decay_t<decltype(v2)> == false ) {
					throw std::runtime_error( "Unreachable code" );
					return false;
				} else
					return cxxmath::less( v1, v2 );
			}
		);
	}
};

struct less_gamma_matrix {
	static constexpr apply( const mathutils::gamma_matrix &gm1, const mathutils::gamma_matrix &gm2 ) {
		return cxxmath::less( gm1.index, gm2.index );
	}
};

struct less_spacetime_index {
	static constexpr apply( const mathutils::spacetime_index &si1, const mathutils::spacetime_index &si2 ) {
		if( si1.id.index() < si2.id.index() )
			return true;
		if( si1.id.index() > si2.id.index() )
			return true;
		
		if( si1.variance == mathutils::spacetime_index::index_variance::lower ) {
			if( si2.variance == mathutils::spacetime_index::index_variance::upper )
				return true;
		}
		
		return false;
	}
};

using model_parameter_total_order = cxxmath::concepts::total_order<less_model_parameters>;
using spacetime_index_total_order = cxxmath::concepts::total_order<less_spacetime_index>;
using gamma_matrix_total_order = cxxmath::concepts::total_order<less_gamma_matrix>;
using lagrangian_polynomial_symbol_total_order = cxxmath::concepts::total_order<less_lagrangian_polynomial_symbol>;
}

namespace cxxmath::impl {
template<> struct default_total_order<model::indexed_parameter> {
	using type = cxxmath_compatibility::model_parameter_total_order;
};
}

namespace PQuantum::calculation {
static constexpr auto simple_field_multiplication_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<indexed_field>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::spacetime_derivative>, boost::hana::int_c<1> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::dirac_operator>, boost::hana::int_c<1> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::field_multiplication_operator>, boost::hana::int_c<1> )
);

using lagrangian_field_multiplication_spec = std::pair<
    std::vector<
        std::variant<
			mathutils::spacetime_derivative,
			mathutils::dirac_operator,
			mathutils::field_multiplication_operator
		>
	>,
	indexed_field
>;

struct lagrangian_polynomial_symbol {
	std::variant<lagrangian_field_multiplication_spec, mathutils::gamma_matrix> data;
};
using lagrangian_polynomial_coefficient = cxxmath::free_r_algebra<
	mathutils::number,
	model::indexed_parameter
>;
using lagrangian_polynomial = free_r_algebra<lagrangian_polynomial_coefficient, lagrangian_polynomial_symbol>;
}

#endif //PQUANTUM_CALCULATION_LAGRANGIAN_POLYNOMIAL_HPP
