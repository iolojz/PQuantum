//
// Created by jayz on 02.02.20.
//

#include "identifier.hpp"

#include "../lazy_instantiations.hpp"

namespace PQuantum::parsing {
template<class TreeNode>
auto evaluate_type_rule(boost::hana::basic_type<support::tree::node_incarnation<identifier, TreeNode>>) {
	return as<std::string>(+boost::spirit::x3::alnum);
}
}
