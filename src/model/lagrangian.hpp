//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "mathutils/expressions.hpp"
#include "mathutils/linear_operators.hpp"

#include "classical_field.hpp"

#include <boost/property_tree/ptree_fwd.hpp>
#include <variant>

namespace PQuantum::model
{
struct lagrangian_symbol {
	std::variant<mathutils::linear_operator, classical_field_id> value;
};
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
