//
// Created by jayz on 02.02.20.
//

#include "arithmetic.hpp"

namespace PQuantum::support::parsing {
template<class TreeNode>
struct rule_for_impl<tree::node_incarnation<mathutils::tree_expression::parentheses, TreeNode>> {
	static constexpr const char *name = "parentheses";
	
	static auto apply( void ) {
		using child_container = typename tree::node_incarnation<
			mathutils::tree_expression::parentheses, TreeNode
		>::child_container;
		
		auto child_rule = rule_for<typename child_container::value_type>();
		auto children_rule = as<child_container>( '(' >> child_rule >> ')' );
		return boost::spirit::x3::attr( mathutils::tree_expression::parentheses{} ) >> children_rule;
	}
};
}
