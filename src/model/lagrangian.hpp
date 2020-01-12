//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "mathutils/expression_node.hpp"
#include "mathutils/linear_operators.hpp"

#include "classical_field.hpp"

#include <boost/property_tree/ptree_fwd.hpp>
#include <variant>

namespace PQuantum::model
{
using lagrangian_node_data = std::variant<mathutils::node_data::sum, mathutils::node_data::difference, mathutils::node_data::product, mathutils::node_data::quotient, mathutils::node_data::gamma_matrix, mathutils::node_data::sigma_matrix, mathutils::node_data::spacetime_derivative, mathutils::node_data::dirac_operator, classical_field_id, variable_id>;




struct lagrangian_symbol
{
	std::variant<mathutils::linear_operator, classical_field_id> value;
};

std::ostream &operator<<( std::ostream &os, const lagrangian_symbol &ls )
{
	if( std::holds_alternative<mathutils::linear_operator>( ls.value ))
		return os << std::get<mathutils::linear_operator>( ls.value );
	else
		return os << std::get<classical_field_id>( ls.value );
}

using lagrangian_coefficient = mathutils::polynomial_expression;

namespace model_lagrangian_symbol
{
struct less
{
	static constexpr bool
	apply( const PQuantum::model::lagrangian_symbol &ls1, const PQuantum::model::lagrangian_symbol &ls2 ) noexcept
	{
		return ls1.value < ls2.value;
	}
};
}
}

namespace cxxmath::impl
{
template<>
struct default_total_order<PQuantum::model::lagrangian_symbol>
{
	using type = concepts::total_order<PQuantum::model::model_lagrangian_symbol::less>;
};
}

namespace PQuantum::model {
using lagrangian = cxxmath::free_r_algebra<lagrangian_coefficient, lagrangian_symbol>;

static constexpr auto make_lagrangian = cxxmath::make<cxxmath::tag_of_t<lagrangian>>;
}

BOOST_FUSION_ADAPT_STRUCT(PQuantum::model::lagrangian_symbol, value)

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
