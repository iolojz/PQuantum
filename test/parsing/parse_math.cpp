//
// Created by jayz on 16.01.20.
//

// #define BOOST_SPIRIT_X3_DEBUG

#include <sstream>

#include "mathutils/tree_expression/arithmetic.hpp"
#include "support/parsing.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;
using namespace PQuantum::support;
using namespace PQuantum::mathutils;

BOOST_AUTO_TEST_CASE( parse_math ) {
	std::string test = "a + b - c - d * e / (f + g)";
	auto result = parsing::parse<tree_expression::arithmetic_tree>( test.begin(), test.end());
	
	BOOST_TEST_REQUIRE( result.index() == 0, "test string could not be parsed" );
	auto root_incarnation = std::get<0>( result ).node_incarnation;
	
	std::stringstream ss;
	ss << root_incarnation;
	BOOST_TEST( test == ss.str() );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::difference>( root_incarnation ) );
	auto root_difference = tree::get_node_incarnation<tree_expression::difference>( root_incarnation );
	
	BOOST_TEST_REQUIRE( std::size( root_difference.children ) == 3 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::sum>( root_difference.children[0] ) );
	auto a_plus_b = tree::get_node_incarnation<tree_expression::sum>( root_difference.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( root_difference.children[1] ) );
	auto c = tree::get_node_incarnation<tree_expression::terminal_string>( root_difference.children[1] );
	
	BOOST_TEST( c.data.data == "c" );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::quotient>( root_difference.children[2] ) );
	auto tail = tree::get_node_incarnation<tree_expression::quotient>( root_difference.children[2] );
	
	BOOST_TEST_REQUIRE( std::size( tail.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::product>( tail.children[0] ) );
	auto d_times_e = tree::get_node_incarnation<tree_expression::product>( tail.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::parentheses>( tail.children[1] ) );
	auto parentheses = tree::get_node_incarnation<tree_expression::parentheses>( tail.children[1] );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::sum>( parentheses.child ) );
	auto f_plus_g = tree::get_node_incarnation<tree_expression::sum>( parentheses.child );
	
	BOOST_TEST_REQUIRE( std::size( a_plus_b.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( a_plus_b.children[0] ) );
	auto a = tree::get_node_incarnation<tree_expression::terminal_string>( a_plus_b.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( a_plus_b.children[1] ) );
	auto b = tree::get_node_incarnation<tree_expression::terminal_string>( a_plus_b.children[1] );
	
	BOOST_TEST( a.data.data == "a" );
	BOOST_TEST( b.data.data == "b" );
	
	BOOST_TEST_REQUIRE( std::size( d_times_e.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( d_times_e.children[0] ) );
	auto d = tree::get_node_incarnation<tree_expression::terminal_string>( d_times_e.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( d_times_e.children[1] ) );
	auto e = tree::get_node_incarnation<tree_expression::terminal_string>( d_times_e.children[1] );
	
	BOOST_TEST( d.data.data == "d" );
	BOOST_TEST( e.data.data == "e" );
	
	BOOST_TEST_REQUIRE( std::size( f_plus_g.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( f_plus_g.children[0] ) );
	auto f = tree::get_node_incarnation<tree_expression::terminal_string>( f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<tree_expression::terminal_string>( f_plus_g.children[1] ) );
	auto g = tree::get_node_incarnation<tree_expression::terminal_string>( f_plus_g.children[1] );
	
	BOOST_TEST( f.data.data == "f" );
	BOOST_TEST( g.data.data == "g" );
}