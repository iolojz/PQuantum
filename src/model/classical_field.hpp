//
// Created by jayz on 01.08.19.
//

#ifndef PQUANTUM_CLASSICAL_FIELD_HPP
#define PQUANTUM_CLASSICAL_FIELD_HPP

#include "mathutils/manifold_specification.hpp"

#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

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

using classical_field_id = boost::uuids::uuid
id;
}
}

#endif //PQUANTUM_CLASSICAL_FIELD_HPP
