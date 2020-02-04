//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
#define PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP

#include <string>

#include "support/tree.hpp"

#include "../x3_helpers.hpp"

namespace PQuantum::parsing {
struct identifier {
	std::string name;
};

struct identifier_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<identifier>;
};

template<class TreeTag>
struct type_rule_impl<support::tree::node_incarnation<PQuantum::parsing::identifier, TreeTag>> {
	static auto apply( void ) { return as<std::string>(+boost::spirit::x3::alnum); }
};
}

namespace PQuantum::support::tree {
template<class TreeTag>
std::ostream &operator<<( std::ostream &os, const node_incarnation<PQuantum::parsing::identifier, TreeTag> &ni ) {
	return os << ni.data.name;
}
}

BOOST_FUSION_ADAPT_STRUCT(PQuantum::parsing::identifier, name)

PQUANTUM_TREE_DEFINE_NODE_ARITY(PQuantum::parsing::identifier,0)

#endif //PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
