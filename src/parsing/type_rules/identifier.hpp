//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
#define PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP

#include <string>

#include "support/tree.hpp"

namespace PQuantum::parsing {
struct identifier {
	std::string name;
};

struct identifier_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<identifier>;
};

template<class TreeNode>
auto evaluate_type_rule( boost::hana::basic_type<support::tree::node_incarnation<identifier, TreeNode>> );
}

namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<PQuantum::parsing::identifier, TreeNode> &ni ) {
	return os << ni.data.name;
}
}

BOOST_FUSION_ADAPT_STRUCT(PQuantum::parsing::identifier, name)

PQUANTUM_TREE_DEFINE_NODE_ARITY(PQuantum::parsing::identifier,0)

#endif //PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
