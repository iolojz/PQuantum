//
// Created by jayz on 16.01.20.
//

#include <sstream>

#include "logging/logging.hpp"

#include "parsing/parse_math.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;
using namespace PQuantum::support;
using namespace PQuantum::mathutils;

namespace {
struct logging_setup {
	logging_setup( void ) {
		logging::setup_logging_facilities();
	}
};
}

BOOST_TEST_GLOBAL_FIXTURE( logging_setup );

BOOST_AUTO_TEST_CASE( parse_math ) {
	using indexed_string_atom = atom_with_optional_indices<string_atom>;
	
	std::string test = "a + b - c - d * e / (f + g)";
	auto root_incarnation = parsing::parse_math( test );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<difference>( root_incarnation ) );
	auto root_difference = tree::get_node_incarnation<difference>( root_incarnation );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<difference>( root_difference.children[0] ) );
	auto a_plus_b_minus_c = tree::get_node_incarnation<difference>( root_difference.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<quotient>( root_difference.children[1] ) );
	auto d_times_e_over_f_plus_g = tree::get_node_incarnation<quotient>( root_difference.children[1] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<sum>( a_plus_b_minus_c.children[0] ) );
	auto a_plus_b = tree::get_node_incarnation<sum>( a_plus_b_minus_c.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( a_plus_b_minus_c.children[1] ) );
	auto c = tree::get_node_incarnation<indexed_string_atom>( a_plus_b_minus_c.children[1] );
	
	BOOST_TEST( c.data.atom.name == "c" );
	BOOST_TEST( c.data.indices.lower.size() == 0 );
	BOOST_TEST( c.data.indices.upper.size() == 0 );
	
	BOOST_TEST_REQUIRE( a_plus_b.children.size() == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( a_plus_b.children[0] ) );
	auto a = tree::get_node_incarnation<indexed_string_atom>( a_plus_b.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( a_plus_b.children[1] ) );
	auto b = tree::get_node_incarnation<indexed_string_atom>( a_plus_b.children[1] );
	
	BOOST_TEST( a.data.atom.name == "a" );
	BOOST_TEST( a.data.indices.lower.size() == 0 );
	BOOST_TEST( a.data.indices.upper.size() == 0 );
	
	BOOST_TEST( b.data.atom.name == "b" );
	BOOST_TEST( b.data.indices.lower.size() == 0 );
	BOOST_TEST( b.data.indices.upper.size() == 0 );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<product>( d_times_e_over_f_plus_g.children[0] ) );
	auto d_times_e = tree::get_node_incarnation<product>( d_times_e_over_f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<sum>( d_times_e_over_f_plus_g.children[1] ) );
	auto f_plus_g = tree::get_node_incarnation<sum>( d_times_e_over_f_plus_g.children[1] );
	
	BOOST_TEST_REQUIRE( d_times_e.children.size() == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( d_times_e.children[0] ) );
	auto d = tree::get_node_incarnation<indexed_string_atom>( d_times_e.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( d_times_e.children[1] ) );
	auto e = tree::get_node_incarnation<indexed_string_atom>( d_times_e.children[1] );
	
	BOOST_TEST( d.data.atom.name == "d" );
	BOOST_TEST( d.data.indices.lower.size() == 0 );
	BOOST_TEST( d.data.indices.upper.size() == 0 );
	
	BOOST_TEST( e.data.atom.name == "e" );
	BOOST_TEST( e.data.indices.lower.size() == 0 );
	BOOST_TEST( e.data.indices.upper.size() == 0 );
	
	BOOST_TEST_REQUIRE( f_plus_g.children.size() == 2 );
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( f_plus_g.children[0] ) );
	auto f = tree::get_node_incarnation<indexed_string_atom>( f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( tree::holds_node_incarnation<indexed_string_atom>( f_plus_g.children[1] ) );
	auto g = tree::get_node_incarnation<indexed_string_atom>( f_plus_g.children[1] );
	
	BOOST_TEST( f.data.atom.name == "f" );
	BOOST_TEST( f.data.indices.lower.size() == 0 );
	BOOST_TEST( f.data.indices.upper.size() == 0 );
	
	BOOST_TEST( g.data.atom.name == "g" );
	BOOST_TEST( g.data.indices.lower.size() == 0 );
	BOOST_TEST( g.data.indices.upper.size() == 0 );
}