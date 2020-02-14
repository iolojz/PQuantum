//
// Created by jayz on 11.01.20.
//

#ifndef PQUANTUM_MODEL_PARAMETER_HPP
#define PQUANTUM_MODEL_PARAMETER_HPP

#include "support/uuid.hpp"

namespace PQuantum::model {
struct indexed_parameter {
	support::uuid id;
	mathutils::index_spec<std::variant<int, support::uuid>> indices;
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(model::indexed_parameter, 0)

#endif //PQUANTUM_MODEL_PARAMETER_HPP
