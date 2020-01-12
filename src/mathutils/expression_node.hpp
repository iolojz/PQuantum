//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_EXPRESSION_NODE_HPP
#define PQUANTUM_EXPRESSION_NODE_HPP

#include <cxxmath/cxxmath.hpp>

#include "manifold_specification.hpp"
#include "number.hpp"

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

namespace PQuantum::mathutils {
struct variable_id
{
	boost::uuids::uuid id;
};

template<class ...Types>

using expression_node_data = typename std_variant_over_tuple_types<std::decay_t<decltype( std::tuple_cat(
std::declval<linear_operators>, std::tuple<spacetime_dimension, variable_id, classical_field_id, number> ))>::type;

using expression_node = tree_node<expression_node_data>;
}

BOOST_FUSION_ADAPT_STRUCT(PQuantum::mathutils::variable_id, id)

#endif //PQUANTUM_EXPRESSION_NODE_HPP
