//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_TREE_RULES_CHILD_HPP
#define PQUANTUM_PARSING_TREE_RULES_CHILD_HPP

#include "node_rule_for_fwd.hpp"

#include "parse_tree.hpp"

namespace PQuantum::parsing::tree_rules {
namespace detail {
struct child_tag;
}

template<class Iterator, class Context, class Attribute, class Node>
inline bool parse_rule(
	boost::spirit::x3::rule <node_type_tag, Node>,
	Iterator &first, const Iterator &last,
	const Context &context, Attribute &attr
) {
	using boost::spirit::x3::get;
	using boost::spirit::x3::unused;
	auto def = node_rule_for<typename NodeType::node_data>( get<parsing_context_tag>( context ));
	return def.parse( first, last, context, unused, attr );
}

template<class Context>
static auto node_rule_for_child( Context ) {
	using node_type = typename Context::node_type;
	return boost::spirit::x3::rule < node_type_tag, node_type > { "lazy" };
}
}

#endif //PQUANTUM_PARSING_TREE_RULES_CHILD_HPP
