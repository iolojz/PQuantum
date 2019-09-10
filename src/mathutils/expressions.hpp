//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_EXPRESSIONS_HPP
#define PQUANTUM_EXPRESSIONS_HPP

#include "tag_dispatch/models/free_polynomial.hpp"
#include "manifold_specification.hpp"
#include "number.hpp"

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace PQuantum
{
namespace mathutils
{
class variable_id
{
	boost::uuids::uuid id;
public:
	variable_id( void ) : id{ boost::uuids::random_generator()() }
	{}
	
	bool operator==(const variable_id &v) const noexcept
	{ return id == v.id; }
	
	bool operator<(const variable_id &v) const noexcept
	{ return id < v.id; }
};

using expression_variable = std::variant<variable_id, spacetime_dimension>;

namespace detail {
struct less_expression_variables {
	bool operator()(const variable_id &vid1, const variable_id &vid2) const noexcept { return vid1 < vid2; }
	
	constexpr bool operator()(const variable_id &, const spacetime_dimension &) const noexcept { return true; }
	
	constexpr bool operator()(const spacetime_dimension &, const variable_id &) const noexcept { return false; }
	
	constexpr bool operator()(const spacetime_dimension &, const spacetime_dimension &) const noexcept { return false; }
};
}

static constexpr bool operator<(const expression_variable &ev1, const expression_variable &ev2) {
	return std::visit(detail::less_expression_variables{}, ev1, ev2);
}

using polynomial_expression = mathutils::free_polynomial<expression_variable, number>;
}
}

#endif //PQUANTUM_EXPRESSIONS_HPP
