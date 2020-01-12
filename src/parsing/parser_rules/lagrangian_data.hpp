//
// Created by jayz on 10.01.20.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_NODE_DATA_HPP
#define PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_NODE_DATA_HPP

#include "support/tree.hpp"

#include "mathutils/lagrangian.hpp"

namespace PQuantum::parsing::parser_rules
{
template<>
struct rule_for_impl<tree_node < mathutils::node_data::sum>>
{
template<class Context>
auto operator()( Context ) const
{
	auto rule_def = rule_for < tree_node < lagrangian_node_data >>
	boost::spirit::x3::rule<plus_minus_operator_tag, char> rule{ "plus/minus" };
	return ( rule = rule_def );
}
};
}

#endif //PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_NODE_DATA_HPP
