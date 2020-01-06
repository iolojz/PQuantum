//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_EXPRESSIONS_HPP
#define PQUANTUM_EXPRESSIONS_HPP

#include <cxxmath/cxxmath.hpp>

#include "manifold_specification.hpp"
#include "number.hpp"

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

namespace PQuantum::mathutils {
struct imaginary_unit {
	constexpr bool operator<(const imaginary_unit &) const {
		return false;
	}
};

struct variable_id {
	boost::uuids::uuid id;
	
	bool operator==(const variable_id &v) const noexcept { return id == v.id; }
	
	bool operator<(const variable_id &v) const noexcept { return id < v.id; }
};

struct expression_symbol
{
	std::variant<variable_id, spacetime_dimension> value;
};

namespace model_expression_symbol
{
class less
{
	static bool apply( const variable_id &vid1, const variable_id &vid2 ) noexcept
	{ return vid1 < vid2; }
	
	static constexpr bool apply( const variable_id &, const spacetime_dimension & ) noexcept
	{ return true; }
	
	static constexpr bool apply( const spacetime_dimension &, const variable_id & ) noexcept
	{ return false; }
	
	static constexpr bool apply( const spacetime_dimension &, const spacetime_dimension & ) noexcept
	{ return false; }

public:
	static constexpr bool apply( const expression_symbol &s1, const expression_symbol &s2 ) noexcept
	{
		return std::visit( []( auto &&v1, auto &&v2 ) {
			return apply( std::forward<decltype( v1 )>( v1 ), std::forward<decltype( v2 )>( v2 ));
		}, s1.value, s2.value );
	}
};
}
}

namespace cxxmath::impl
{
template<>
struct default_total_order<PQuantum::mathutils::expression_symbol>
{
	using type = concepts::total_order<PQuantum::mathutils::model_expression_symbol::less>;
};
}

namespace PQuantum::mathutils {
using polynomial_expression = cxxmath::free_r_algebra<number, expression_symbol>;

static constexpr auto make_polynomial_expression = cxxmath::make<cxxmath::tag_of_t<mathutils::polynomial_expression>>;
}

BOOST_FUSION_ADAPT_STRUCT(PQuantum::mathutils::imaginary_unit)
BOOST_FUSION_ADAPT_STRUCT(PQuantum::mathutils::variable_id, id)
BOOST_FUSION_ADAPT_STRUCT(PQuantum::mathutils::expression_symbol, value)

#endif //PQUANTUM_EXPRESSIONS_HPP
