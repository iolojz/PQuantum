//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
#define PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP

#include <string>

#include "support/tree.hpp"

#include "../x3_helpers.hpp"
#include "../tree_helpers.hpp"

namespace PQuantum::parsing {
struct identifier {
	std::string name;
};

struct identifier_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<identifier>;
};

template<class TreeTag>
struct type_rule_impl<support::tree::node_incarnation<PQuantum::parsing::identifier, TreeTag>> {
	static auto apply( void ) {
		using boost::spirit::x3::alnum;
		using boost::spirit::x3::char_;
		using boost::spirit::x3::string;
		
		return as<std::string>(
			+(alnum | char_('\\') | char_('{') | char_('}') | char_('_') | char_('^'))
		);
	}
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

PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(identifier)

#endif //PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
