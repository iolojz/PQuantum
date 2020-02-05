//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_ARITHMETIC_HPP
#define PQUANTUM_PARSING_ARITHMETIC_HPP

#include "mathutils/tree_expression/arithmetic.hpp"

#include "arithmetic_core.hpp"
#include "variant.hpp"

#include "../tree_helpers.hpp"

namespace PQuantum::parsing::detail {
template<class TreeNode, class Op>
static constexpr auto child_type_of_op_for_parsing( boost::hana::basic_type<Op> op ) {
	using child_container = typename support::tree::node_incarnation<
		Op, TreeNode
	>::child_container;
	constexpr auto all_child_incarnations = boost::hana::remove(
		boost::hana::to_tuple( typename child_container::value_type::types{} ),
		boost::hana::type_c<boost::blank>
	);
	
	constexpr auto valid_child_incarnations = boost::hana::filter(
		all_child_incarnations,
		[op] (auto &&incarnation) {
			constexpr auto node_data = boost::hana::type_c<
			    typename boost::unwrap_recursive<typename decltype(+incarnation)::type::node_data>::type
			>;
			
			if constexpr( mathutils::is_arithmetic_op( node_data ))
				return mathutils::has_higher_precedence( node_data, op );
			else
				return boost::hana::true_c;
		}
	);
	
	return boost::hana::unpack(
		valid_child_incarnations,
		boost::hana::template_<boost::variant>
	);
}
}

#ifdef PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR
#error "PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR is already defined"
#endif
#define PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(op_name, op) \
namespace PQuantum::parsing { \
template<class TreeTag> \
struct type_rule_impl<support::tree::node_incarnation<mathutils::op_name, TreeTag>> { \
	static auto apply( void ) { \
		using child_container = typename support::tree::node_incarnation< \
		    mathutils::op_name, TreeTag \
		>::child_container; \
		constexpr auto child = detail::child_type_of_op_for_parsing<TreeTag>( \
			boost::hana::type_c<mathutils::op_name> \
		); \
		\
		auto child_rule = make_type_rule<typename decltype(+child)::type>(); \
		auto children_rule = as<child_container>( child_rule >> +( op >> child_rule )); \
		return boost::spirit::x3::attr( mathutils::op_name{} ) >> children_rule; \
	} \
}; \
}

PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(mathutils::product)
PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(mathutils::quotient)
PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(mathutils::sum)
PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(mathutils::difference)
PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(mathutils::parentheses)

PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(product,'*')
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(quotient,'/')
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(sum,'+')
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(difference,'-')

namespace PQuantum::parsing {
template<class TreeTag>
struct type_rule_impl<support::tree::node_incarnation<mathutils::parentheses, TreeTag>> {
	static auto apply(void) {
		using child_container = typename support::tree::node_incarnation<mathutils::parentheses, TreeTag>::child_container;
		
		auto child_rule = make_type_rule<typename child_container::value_type>();
		auto children_rule = as<child_container>('(' >> child_rule >> ')');
		return boost::spirit::x3::attr(mathutils::parentheses{}) >> children_rule;
	}
};
}

#endif //PQUANTUM_PARSING_ARITHMETIC_HPP
