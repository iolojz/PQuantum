//
// Created by jayz on 02.02.20.
//

#ifndef PQUANTUM_PARSING_ARITHMETIC_HPP
#define PQUANTUM_PARSING_ARITHMETIC_HPP

#include "mathutils/tree_expression/arithmetic.hpp"

#include "identifier.hpp"
#include "variant.hpp"

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

#ifdef PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR
#error "PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR is already defined"
#endif
#define PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(op_name, op) \
namespace PQuantum::parsing { \
template<class TreeTag> \
struct type_rule_impl<support::tree::node_incarnation<mathutils::tree_expression::op_name, TreeTag>> { \
	static auto apply( void ) { \
		using child_container = typename support::tree::node_incarnation< \
		    mathutils::tree_expression::op_name, TreeTag \
		>::child_container; \
		constexpr auto child = detail::child_type_of_op_for_parsing<TreeTag>( \
			boost::hana::type_c<mathutils::tree_expression::op_name> \
		); \
		\
		auto child_rule = make_type_rule<typename decltype(+child)::type>(); \
		auto children_rule = as<child_container>( child_rule % op ); \
		return boost::spirit::x3::attr( mathutils::tree_expression::op_name{} ) >> children_rule; \
	} \
}; \
}

PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(product,'*')
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(quotient,'/')
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(sum,'+')
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR(difference,'-')

namespace PQuantum::parsing {
template<class TreeTag>
struct type_rule_impl<support::tree::node_incarnation<mathutils::tree_expression::parentheses, TreeTag>> {
	static auto apply(void) {
		using child_container = typename support::tree::node_incarnation<mathutils::tree_expression::parentheses, TreeTag>::child_container;
		
		auto child_rule = make_type_rule<typename child_container::value_type>();
		auto children_rule = as<child_container>('(' >> child_rule >> ')');
		return boost::spirit::x3::attr(mathutils::tree_expression::parentheses{}) >> children_rule;
	}
};
}

#endif //PQUANTUM_PARSING_ARITHMETIC_HPP
