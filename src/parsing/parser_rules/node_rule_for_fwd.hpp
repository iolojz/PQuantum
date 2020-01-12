//
// Created by jayz on 23.08.19.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_RULE_FOR_FWD_HPP
#define PQUANTUM_PARSING_PARSER_RULES_RULE_FOR_FWD_HPP

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing::parser_rules
{
template<class NodeType, class NodeData, class = void>
struct node_rule_for_impl;

template<class NodeType, class NodeData> static constexpr auto node_rule_for = []( auto context ) {
	auto rule_and_name = node_rule_for_impl<NodeType, NodeData>::apply( context );
	return ( boost::spirit::x3::rule<struct _, NodeType>{ rule_and_name.second } = rule_and_name.first );
}

#endif //PQUANTUM_PARSING_PARSER_RULES_RULE_FOR_FWD_HPP
