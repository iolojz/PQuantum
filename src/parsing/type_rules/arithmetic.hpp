//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_ARITHMETIC_HPP
#define PQUANTUM_ARITHMETIC_HPP

#include "mathutils/tree_expression/arithmetic.hpp"
#include "identifier.hpp"

#include "../type_rule.hpp"

namespace PQuantum::parsing::detail {
template<class TreeNode, class Op>
static constexpr auto child_type_of_op_for_parsing( boost::hana::basic_type<Op> op ) {
	using child_container = typename support::tree::node_incarnation<
		Op, TreeNode
	>::child_container;
	using all_child_incarnations = typename child_container::value_type::types;
	
	constexpr auto valid_child_incarnations = boost::hana::filter(
		boost::hana::to_tuple( all_child_incarnations{} ),
		[op] (auto &&incarnation) {
			constexpr auto node_data = boost::hana::type_c<
				typename boost::unwrap_recursive<typename decltype(+incarnation)::type::node_data>::type
			>;
			
			if constexpr( mathutils::tree_expression::is_arithmetic_op( node_data ))
				return mathutils::tree_expression::has_higher_precedence( node_data, op );
			else
				return boost::hana::true_c;
		} );
	
	return boost::hana::unpack(
		valid_child_incarnations,
		boost::hana::template_<boost::variant>
	);
}
}

#ifdef PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_OPERATOR
#error "PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_OPERATOR is already defined"
#endif
#define PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_NODE( node_name ) \
namespace PQuantum::parsing { \
template<class TreeNode> \
auto evaluate_type_rule( \
    boost::hana::basic_type<support::tree::node_incarnation<mathutils::tree_expression::node_name, TreeNode>> \
); \
}

#ifdef PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR
#error "PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR is already defined"
#endif
#define PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( op_name, op ) \
namespace PQuantum::parsing { \
template<class TreeNode> \
auto evaluate_type_rule( \
	boost::hana::basic_type<support::tree::node_incarnation<mathutils::tree_expression::op_name, TreeNode>> \
) { \
    using child_container = typename support::tree::node_incarnation< \
    	mathutils::tree_expression::op_name, TreeNode \
    >::child_container; \
    constexpr auto child = detail::child_type_of_op_for_parsing<TreeNode>( \
    	boost::hana::type_c<mathutils::tree_expression::op_name> \
    ); \
    \
    auto child_rule = rule_for<typename decltype(+child)::type>(); \
    auto children_rule = as<child_container>( child_rule % op ); \
    return boost::spirit::x3::attr( mathutils::tree_expression::op_name{} ) >> children_rule; \
} \
}

PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_NODE( product )
PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_NODE( quotient )
PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_NODE( sum )
PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_NODE( difference )
PQUANTUM_DECLARE_RULE_FOR_ARITHMETIC_NODE( parentheses )

#endif //PQUANTUM_ARITHMETIC_HPP
