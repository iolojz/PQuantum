//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_EXPRESSIONS_HPP
#define PQUANTUM_EXPRESSIONS_HPP

#include "free_polynomial.hpp"
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
	
	bool operator==( const variable_id &v ) const
	{ return id == v.id; }
	
	bool operator<( const variable_id &v ) const
	{ return id < v.id; }
};

using expression_variable = std::variant<variable_id, spacetime_dimension>;

using polynomial_expression = mathutils::free_polynomial<expression_variable, number>;
}
}

#endif //PQUANTUM_EXPRESSIONS_HPP
