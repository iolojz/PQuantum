//
// Created by jayz on 11.01.20.
//

#ifndef PQUANTUM_MODEL_PARAMETER_HPP
#define PQUANTUM_MODEL_PARAMETER_HPP

#include "support/uuid.hpp"

namespace PQuantum::model {
struct parameter {
	support::uuid id;
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(model::parameter, 0)

#endif //PQUANTUM_MODEL_PARAMETER_HPP
