//
// Created by jayz on 16.01.20.
//

// #define BOOST_SPIRIT_X3_DEBUG

#include <sstream>

#include "error/error.hpp"

#include "logging/logging.hpp"

#include "mathutils/tree_expression/arithmetic.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

namespace {
struct terminal_string {
	std::string str;
};

struct string_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<terminal_string>;
};
}

namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<terminal_string, TreeNode> &ni ) {
	return os << ni.data.str;
}
}

BOOST_FUSION_ADAPT_STRUCT( terminal_string, str )

PQUANTUM_TREE_DEFINE_NODE_ARITY(terminal_string,0)

namespace PQuantum::support::parsing {
template<class TreeNode>
struct rule_for_impl<tree::node_incarnation<terminal_string, TreeNode>> {
	static constexpr const char *name = "terminal_string";
	
	static auto apply( void ) {
		return as<std::string>( +boost::spirit::x3::alpha );
	}
};
}

namespace {
using arithmetic_node = PQuantum::support::tree::tree_node<
    PQuantum::mathutils::tree_expression::arithmetic_node_traits,
    string_node_traits
>;
}

using namespace PQuantum;
using namespace PQuantum::support;
using namespace PQuantum::mathutils;

namespace {
arithmetic_node parse_arithmetic_expression( const std::string &str ) {
	BOOST_LOG_NAMED_SCOPE( "parse_arithmetic_expression" );
	logging::severity_logger logger;
	
	auto result = support::parsing::parse<arithmetic_node>( str.begin(), str.end() );
	if( result.index() == 0 )
		return std::get<0>( result );
	
	auto parsing_result_tuple = std::get<1>( result );
	if( std::get<0>( parsing_result_tuple ) ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error )
			<< "End of input string is not an arithmetic expression: \""
			<< std::string( std::get<1>( parsing_result_tuple ), std::get<2>( parsing_result_tuple ) )
			<< "\"";
		error::exit_upon_error();
	}
	
	BOOST_LOG_SEV( logger, logging::severity_level::error )
		<< "Input string is not an arithmetic expression: \"" << str << "\"";
	error::exit_upon_error();
}
}

BOOST_AUTO_TEST_CASE( parse_math ) {
	std::string test = "a + b - c - d * e / (f + g)";
	auto root_incarnation = parse_arithmetic_expression( test );
	
	std::stringstream ss;
	ss << root_incarnation;
	BOOST_TEST( test == ss.str() );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::difference>( root_incarnation ) );
	auto root_difference = tree::get_node_incarnation<tree_expression::difference>( root_incarnation );
	
	BOOST_TEST_REQUIRE( std::size( root_difference.children ) == 3 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::sum>( root_difference.children[0] ) );
	auto a_plus_b = tree::get_node_incarnation<tree_expression::sum>( root_difference.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( root_difference.children[1] ) );
	auto c = tree::get_node_incarnation<terminal_string>( root_difference.children[1] );
	
	BOOST_TEST( c.data.str == "c" );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::quotient>( root_difference.children[2] ) );
	auto tail = tree::get_node_incarnation<tree_expression::quotient>( root_difference.children[2] );
	
	BOOST_TEST_REQUIRE( std::size( tail.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::product>( tail.children[0] ) );
	auto d_times_e = tree::get_node_incarnation<tree_expression::product>( tail.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::parentheses>( tail.children[1] ) );
	auto parentheses = tree::get_node_incarnation<tree_expression::parentheses>( tail.children[1] );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::sum>( parentheses.children.front() ) );
	auto f_plus_g = tree::get_node_incarnation<tree_expression::sum>( parentheses.children.front() );
	
	BOOST_TEST_REQUIRE( std::size( a_plus_b.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( a_plus_b.children[0] ) );
	auto a = tree::get_node_incarnation<terminal_string>( a_plus_b.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( a_plus_b.children[1] ) );
	auto b = tree::get_node_incarnation<terminal_string>( a_plus_b.children[1] );
	
	BOOST_TEST( a.data.str == "a" );
	BOOST_TEST( b.data.str == "b" );
	
	BOOST_TEST_REQUIRE( std::size( d_times_e.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( d_times_e.children[0] ) );
	auto d = tree::get_node_incarnation<terminal_string>( d_times_e.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( d_times_e.children[1] ) );
	auto e = tree::get_node_incarnation<terminal_string>( d_times_e.children[1] );
	
	BOOST_TEST( d.data.str == "d" );
	BOOST_TEST( e.data.str == "e" );
	
	BOOST_TEST_REQUIRE( std::size( f_plus_g.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( f_plus_g.children[0] ) );
	auto f = tree::get_node_incarnation<terminal_string>( f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<terminal_string>( f_plus_g.children[1] ) );
	auto g = tree::get_node_incarnation<terminal_string>( f_plus_g.children[1] );
	
	BOOST_TEST( f.data.str == "f" );
	BOOST_TEST( g.data.str == "g" );
}