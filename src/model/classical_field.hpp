//
// Created by jayz on 01.08.19.
//

#ifndef PQUANTUM_CLASSICAL_FIELD_HPP
#define PQUANTUM_CLASSICAL_FIELD_HPP

#include "mathutils/manifold_specification.hpp"
#include "mathutils/atom_indices.hpp"

#include "spacetime_index.hpp"

#include <string>

#include "support/uuid.hpp"

namespace PQuantum::model {
struct classical_field_specification {
	std::string name;
	mathutils::manifold_types::vector_space::algebraic_field algebraic_field;
	std::variant<int, mathutils::spacetime_dimension, mathutils::clifford_algebra_target_dimension> dimension;
};

struct indexed_field {
	support::uuid id;
	mathutils::index_spec<std::variant<int, support::uuid>> indices;
};
}

#endif //PQUANTUM_CLASSICAL_FIELD_HPP
