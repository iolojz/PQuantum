//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "classical_field.hpp"
#include "spacetime_index.hpp"
#include "parameter.hpp"

#include "mathutils/string_atom.hpp"
#include "mathutils/arithmetic.hpp"
#include "mathutils/function_call.hpp"
#include "mathutils/number.hpp"

namespace PQuantum::model {
struct euler_constant {
	constexpr bool operator==( const euler_constant & ) const { return true; }
};

struct pi_constant {
	constexpr bool operator==( const pi_constant & ) const { return true; }
};

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

static constexpr auto lagrangian_atom_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<indexed_field>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<indexed_parameter>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<gamma_matrix>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<spacetime_derivative>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<dirac_operator>, boost::hana::int_c<0> ),
	boost::hana::make_pair( boost::hana::type_c<mathutils::number>, boost::hana::int_c<0> )
);
static constexpr auto lagrangian_arity_map = boost::hana::union_(
	boost::hana::union_(
		mathutils::function_call_arity_map<mathutils::string_atom>,
		mathutils::arithmetic_arity_map
	),
	lagrangian_atom_arity_map
);
using lagrangian_tree = cxxmath::typesafe_tree<decltype(lagrangian_arity_map)>;

static constexpr auto lagrangian_delta_field_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<delta_indexed_field>, boost::hana::int_c<0> )
);
static constexpr auto delta_lagrangian_arity_map = boost::hana::union_(
	lagrangian_arity_map,
	lagrangian_delta_field_arity_map
);
using delta_lagrangian_tree = cxxmath::typesafe_tree<decltype(delta_lagrangian_arity_map)>;

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const euler_constant & ) {
	return os << "\\EulerConstant";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const pi_constant & ) {
	return os << "\\pi";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const gamma_matrix &gm ) {
	return os << "\\gamma" << gm.index;
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const spacetime_derivative &sd ) {
	return os << "\\SpacetimeDerivative" << sd.index;
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const dirac_operator & ) {
	return os << "\\DiracOperator";
}
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
