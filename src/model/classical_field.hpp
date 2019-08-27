//
// Created by jayz on 01.08.19.
//

#ifndef PQUANTUM_CLASSICAL_FIELD_HPP
#define PQUANTUM_CLASSICAL_FIELD_HPP

#include "mathutils/manifold_specification.hpp"

#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace PQuantum
{
namespace model
{
struct classical_field_specification
{
	std::string name;
	mathutils::manifold_types::vector_space::algebraic_field algebraic_field;
	std::variant<int, mathutils::spacetime_dimension> dimension;
};

struct classical_field_id
{
	boost::uuids::uuid id;
	
	bool operator<( const classical_field_id &field_id ) const
	{
		return id < field_id.id;
	}
	
	bool operator==(const classical_field_id &field_id) const {
		return id == field_id.id;
	}
};
}
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::model::classical_field_id, id )

#endif //PQUANTUM_CLASSICAL_FIELD_HPP
