//
// Created by jayz on 07.04.20.
//

#ifndef PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP
#define PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP

#include "model/lagrangian.hpp"

namespace PQuantum::calculation {
struct euler_constant {
	constexpr bool operator==( const euler_constant & ) const { return true; }
};

struct pi_constant {
	constexpr bool operator==( const pi_constant & ) const { return true; }
};

struct delta_indexed_field {
	model::indexed_field field;
	int delta_index;
	
	constexpr bool operator==( const delta_indexed_field &other ) const {
		return (field == other.field && delta_index == other.delta_index);
	}
};

static constexpr auto lagrangian_delta_field_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<delta_indexed_field>, boost::hana::int_c<0> )
);
static constexpr auto delta_lagrangian_arity_map = boost::hana::union_(
	lagrangian_delta_field_arity_map,
	model::lagrangian_arity_map
);
using delta_lagrangian_tree = cxxmath::typesafe_tree<decltype(delta_lagrangian_arity_map)>;

/*delta_lagrangian_tree to_delta_lagrangian( const model::lagrangian_tree &tree );

delta_lagrangian_tree take_nth_derivative( int n, const model::lagrangian_tree &lagrangian );*/
delta_lagrangian_tree take_nth_derivative( int n, const delta_lagrangian_tree &delta_lagrangian );

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const euler_constant & ) {
	return os << "\\EulerConstant";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const pi_constant & ) {
	return os << "\\pi";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const delta_indexed_field &difield ) {
	return os << "\\Delta{" << difield.field << "}_" << difield.delta_index << difield.field.indices;
}
}

#endif //PQUANTUM_CALCULATION_LAGRANGIAN_MANIPULATIONS_HPP
