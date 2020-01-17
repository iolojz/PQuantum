//
// Created by jayz on 16.01.20.
//

#ifndef PQUANTUM_MATHUTILS_TREE_EXPRESSION_ARITHMETIC_HPP
#define PQUANTUM_MATHUTILS_TREE_EXPRESSION_ARITHMETIC_HPP

#include "support/tree.hpp"

#include <boost/hana.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <string>

namespace PQuantum::mathutils::tree_expression {
struct product;
struct quotient;
struct sum;
struct difference;
struct parentheses;
struct terminal_string;

struct terminal_string {
	std::string data;
};

static constexpr auto possible_node_data = boost::hana::tuple_t<
	product, quotient, sum,
	difference, parentheses, terminal_string
>;

static constexpr auto ops_by_descending_precedence = boost::hana::tuple_t<
	product, quotient, sum, difference
>;

static constexpr auto has_higher_precedence = []( auto hop, auto lop ) {
	auto lower_ops = boost::hana::drop_while(
		ops_by_descending_precedence,
		boost::hana::not_equal.to( lop )
	);
	return boost::hana::not_( boost::hana::contains( lower_ops, hop ));
};

static constexpr auto is_op = boost::hana::curry<2>( boost::hana::contains )( ops_by_descending_precedence );

using arithmetic_tree = typename decltype(+boost::hana::unpack(
	boost::hana::concat(
		boost::hana::reverse( ops_by_descending_precedence ),
		boost::hana::remove_if(
			possible_node_data,
			is_op
		)
	),
	boost::hana::template_<support::tree::tree_node>
))::type;

namespace detail {
static constexpr auto higher_operator_filter = []( [[maybe_unused]] auto op_data, auto node_data ) {
	if constexpr( is_op( node_data ))
		return has_higher_precedence( node_data, op_data );
	else
		return boost::hana::true_c;
};
}

#ifdef PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA
#error "PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA is already defined"
#endif
#define PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA( name ) \
struct name { \
    static constexpr auto tree_node_arity = support::tree::runtime_arity; \
    template<class PossibleChildDataTypes> \
    static constexpr auto valid_child_data_types( PossibleChildDataTypes possible_types ) { \
        return boost::hana::filter( \
            possible_types, \
            boost::hana::curry<2>( detail::higher_operator_filter )( boost::hana::type_c<name> ) \
        ); \
    } \
};

PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA( product )
PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA( quotient )
PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA( sum )
PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA( difference )

#undef PQUANTUM_DEFINE_ARITHMETIC_OPERATOR_NODE_DATA

struct parentheses {
	static constexpr std::size_t tree_node_arity = 1;
	
	template<class PossibleChildDataTypes>
	static constexpr auto valid_child_data_types( PossibleChildDataTypes possible_types ) {
		return possible_types;
	}
};
}

namespace PQuantum::support::parsing {
#ifdef PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR
#error "PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR is already defined"
#endif
#define PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( op_name, op ) \
template<class TreeNode> \
struct rule_for_impl<tree::tree_node_incarnation<mathutils::tree_expression::op_name, TreeNode>> { \
    static constexpr const char *name = BOOST_STRINGIZE(op_name); \
    \
    static auto apply( void ) { \
        using incarnation = tree::tree_node_incarnation<mathutils::tree_expression::op_name, TreeNode>; \
        using child_type = typename incarnation::child_type; \
        constexpr auto node_data = typename incarnation::node_data{}; \
        \
        auto child_rule = rule_for<child_type>(); \
        auto op_helper_def = child_rule >> +( op >> child_rule ); \
        boost::spirit::x3::rule<struct _, std::vector<child_type>> op_helper{"op_helper"}; \
        auto op_helper_rule = (op_helper = op_helper_def); \
        \
        return boost::spirit::x3::attr( node_data ) >> op_helper_rule; \
    } \
};

PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( product, '*' )
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( quotient, '/' )
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( sum, '+' )
PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR( difference, '-' )

#undef PQUANTUM_DEFINE_RULE_FOR_ARITHMETIC_OPERATOR

template<class TreeNode>
struct rule_for_impl<tree::tree_node_incarnation<mathutils::tree_expression::parentheses, TreeNode>> {
	static constexpr const char *name = "parentheses";
	
	static auto apply( void ) {
		using incarnation = tree::tree_node_incarnation<mathutils::tree_expression::parentheses, TreeNode>;
		constexpr auto node_data = typename incarnation::node_data{};
		
		auto child_rule = rule_for<typename incarnation::child_type>();
		return boost::spirit::x3::attr( node_data ) >> '(' >> child_rule >> ')';
	}
};

template<>
struct rule_for_impl<mathutils::tree_expression::terminal_string> {
	static constexpr const char *name = "terminal_string";
	
	static auto apply( void ) {
		using boost::spirit::x3::alnum;
		using boost::spirit::x3::char_;
		
		auto valid_char = alnum | char_( '\\' ) | char_( '_' ) | char_( '^' ) | char_( '{' ) | char_( '}' );
		return boost::spirit::x3::lexeme[*valid_char];
	}
};

template<class TreeNode>
struct rule_for_impl<tree::tree_node_incarnation<mathutils::tree_expression::terminal_string, TreeNode>> {
	static constexpr const char *name = "terminal_string_node";
	
	static auto apply( void ) {
		return rule_for<mathutils::tree_expression::terminal_string>();
	}
};
}

namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const tree_node_incarnation<mathutils::tree_expression::product, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"*\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " * " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const tree_node_incarnation<mathutils::tree_expression::quotient, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"/\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " / " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const tree_node_incarnation<mathutils::tree_expression::sum, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"+\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " + " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const tree_node_incarnation<mathutils::tree_expression::difference, TreeNode> &ni ) {
	if( ni.children.empty() )
		throw std::runtime_error( "operator \"-\" has no children" );
	
	auto it = std::begin( ni.children );
	os << *it;
	for( ++it; it != std::end( ni.children ); ++it )
		os << " - " << *it;
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const tree_node_incarnation<mathutils::tree_expression::parentheses, TreeNode> &ni ) {
	return os << "(" << ni.child << ")";
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const tree_node_incarnation<mathutils::tree_expression::terminal_string, TreeNode> &ni ) {
	return os << ni.data.data;
}
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::tree_expression::terminal_string, data )

#endif //PQUANTUM_MATHUTILS_TREE_EXPRESSION_ARITHMETIC_HPP
