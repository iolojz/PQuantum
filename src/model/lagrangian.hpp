//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP

#include "support/tree.hpp"

#include "mathutils/number.hpp"
#include "mathutils/variable.hpp"
#include "mathutils/linear_operators.hpp"

#include "classical_field.hpp"

namespace PQuantum::model {
namespace detail {
static constexpr auto lagrangian_nodes = boost::hana::concat(
	mathutils::linear_operator_types,
	boost::hana::tuple_t<mathutils::variable_id, mathutils::number, classical_field_id>
);
}

// FIXME: ...
/*using lagrangian_tree = decltype(+boost::hana::unpack(
	detail::lagrangian_nodes,
	boost::hana::template_<support::tree::tree_node>
))::type;*/
using lagrangian_tree = support::tree::tree_node<mathutils::linear_operators::sum>;
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
