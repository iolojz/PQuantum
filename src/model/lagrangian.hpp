//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_LAGRANGIAN_HPP
#define PQUANTUM_LAGRANGIAN_HPP

#include "mathutils/expressions.hpp"
#include "mathutils/linear_operators.hpp"

#include "classical_field.hpp"

#include <boost/property_tree/ptree_fwd.hpp>
#include <variant>

namespace PQuantum::model
{
struct lagrangian_symbol
{
	std::variant<mathutils::linear_operator, classical_field_id> value;
};

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

namespace PQuantum::model
{
using lagrangian = cxxmath::free_r_algebra<mathutils::polynomial_expression, lagrangian_symbol>;

static constexpr auto make_lagrangian = cxxmath::make<cxxmath::tag_of_t<lagrangian>>;
}

#endif //PQUANTUM_LAGRANGIAN_HPP
