//
// Created by jayz on 16.01.20.
//

#include <sstream>

#include "parsing/parsing.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;
using namespace PQuantum::support;
using namespace PQuantum::mathutils;

BOOST_AUTO_TEST_CASE( parse_math ) {
	std::string test = "a + b - c - d * e / (f + g)";
	auto root_incarnation = parsing::parse_math( test );
	
	std::stringstream ss;
	ss << root_incarnation;
	BOOST_TEST( test == ss.str() );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<difference>( root_incarnation ) );
	auto root_difference = tree::get_node_incarnation<difference>( root_incarnation );
	
	BOOST_TEST_REQUIRE( std::size( root_difference.children ) == 3 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<sum>( root_difference.children[0] ) );
	auto a_plus_b = tree::get_node_incarnation<sum>( root_difference.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( root_difference.children[1] ) );
	auto c = tree::get_node_incarnation<parsing::atom_with_optional_indices>( root_difference.children[1] );
	
	BOOST_TEST( c.data.name == "c" );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<quotient>( root_difference.children[2] ) );
	auto tail = tree::get_node_incarnation<quotient>( root_difference.children[2] );
	
	BOOST_TEST_REQUIRE( std::size( tail.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<product>( tail.children[0] ) );
	auto d_times_e = tree::get_node_incarnation<product>( tail.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<sum>( tail.children.front() ) );
	auto f_plus_g = tree::get_node_incarnation<sum>( tail.children.front() );
	
	BOOST_TEST_REQUIRE( std::size( a_plus_b.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( a_plus_b.children[0] ) );
	auto a = tree::get_node_incarnation<parsing::atom_with_optional_indices>( a_plus_b.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( a_plus_b.children[1] ) );
	auto b = tree::get_node_incarnation<parsing::atom_with_optional_indices>( a_plus_b.children[1] );
	
	BOOST_TEST( a.data.name == "a" );
	BOOST_TEST( b.data.name == "b" );
	
	BOOST_TEST_REQUIRE( std::size( d_times_e.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( d_times_e.children[0] ) );
	auto d = tree::get_node_incarnation<parsing::atom_with_optional_indices>( d_times_e.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( d_times_e.children[1] ) );
	auto e = tree::get_node_incarnation<parsing::atom_with_optional_indices>( d_times_e.children[1] );
	
	BOOST_TEST( d.data.name == "d" );
	BOOST_TEST( e.data.name == "e" );
	
	BOOST_TEST_REQUIRE( std::size( f_plus_g.children ) == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( f_plus_g.children[0] ) );
	auto f = tree::get_node_incarnation<parsing::atom_with_optional_indices>( f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<parsing::atom_with_optional_indices>( f_plus_g.children[1] ) );
	auto g = tree::get_node_incarnation<parsing::atom_with_optional_indices>( f_plus_g.children[1] );
	
	BOOST_TEST( f.data.name == "f" );
	BOOST_TEST( g.data.name == "g" );
}