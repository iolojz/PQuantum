//
// Created by jayz on 16.01.20.
//

#ifndef PQUANTUM_MATHUTILS_TREE_EXPRESSION_ARITHMETIC_HPP
#define PQUANTUM_MATHUTILS_TREE_EXPRESSION_ARITHMETIC_HPP

#include "support/tree.hpp"
#include "support/parsing.hpp"

#include <boost/hana.hpp>

#include <string>

namespace PQuantum::mathutils::tree_expression {
struct product {};
struct quotient {};
struct sum {};
struct difference {};
struct parentheses {};

static constexpr auto ops_by_precedence = boost::hana::tuple_t<product, quotient, sum, difference>;

struct arithmetic_node_traits {
	static constexpr auto node_data_types = boost::hana::reverse(
		boost::hana::append(
			ops_by_precedence,
			boost::hana::type_c<parentheses>
		)
	);
};

static constexpr auto is_arithmetic_op = boost::hana::curry<2>( boost::hana::contains )( ops_by_precedence );

static constexpr auto has_higher_precedence = []( auto hop, auto lop ) {
	static_assert( is_arithmetic_op( hop ), "hop is not an arithmetic operator." );
	static_assert( is_arithmetic_op( lop ), "hop is not an arithmetic operator." );
	
	auto lower_ops = boost::hana::drop_while(
		ops_by_precedence,
		boost::hana::not_equal.to( lop )
	);
	return boost::hana::not_( boost::hana::contains( lower_ops, hop ));
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::tree_expression::product, runtime_arity)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::tree_expression::quotient, runtime_arity)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::tree_expression::sum, runtime_arity)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::tree_expression::difference, runtime_arity)
PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::tree_expression::parentheses, 1)

namespace PQuantum::support::parsing {
namespace detail {
template<class TreeNode, class Op>
static constexpr auto child_type_of_op_for_parsing( boost::hana::basic_type<Op> op ) {
	using child_container = typename tree::node_incarnation<
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
#define PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( op_name, op ) \
template<class TreeNode> \
struct rule_for_impl<tree::node_incarnation<mathutils::tree_expression::op_name, TreeNode>> { \
	static constexpr const char *name = BOOST_STRINGIZE( op_name ); \
	\
	static auto apply( void ) { \
		using child_container = typename tree::node_incarnation< \
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
};

PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( product, '*' )
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( quotient, '/' )
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( sum, '+' )
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( difference, '-' )

#undef PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR

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


namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::tree_expression::product, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"*\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " * " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::tree_expression::quotient, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"/\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " / " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::tree_expression::sum, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"+\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " + " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::tree_expression::difference, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"-\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " - " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::tree_expression::parentheses, TreeNode> &ni ) {
	return os << "(" << ni.children.front() << ")";
}
}

#endif //PQUANTUM_MATHUTILS_TREE_EXPRESSION_ARITHMETIC_HPP
