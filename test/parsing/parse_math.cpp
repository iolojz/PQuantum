//
// Created by jayz on 16.01.20.
//

#include <sstream>

#include "logging/logging.hpp"

#include "parsing/parse_math.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;
using namespace PQuantum::mathutils;

namespace {
struct logging_setup {
	logging_setup( void ) {
		logging::setup_logging_facilities();
	}
};
}

BOOST_TEST_GLOBAL_FIXTURE( logging_setup );

BOOST_AUTO_TEST_CASE( parse_math_binary ) {
	using indexed_string_atom = atom_with_optional_indices<std::string>;
	
	std::string test = "a + b - c - d * e / (f + g)";
	auto root_incarnation = parsing::parse_math( test );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<difference>( root_incarnation ) );
	auto root_difference = cxxmath::get_node<difference>( root_incarnation );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<difference>( root_difference.children[0] ) );
	auto a_plus_b_minus_c = cxxmath::get_node<difference>( root_difference.children[0] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<quotient>( root_difference.children[1] ) );
	auto d_times_e_over_f_plus_g = cxxmath::get_node<quotient>( root_difference.children[1] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<sum>( a_plus_b_minus_c.children[0] ) );
	auto a_plus_b = cxxmath::get_node<sum>( a_plus_b_minus_c.children[0] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( a_plus_b_minus_c.children[1] ) );
	auto c = cxxmath::get_node<indexed_string_atom>( a_plus_b_minus_c.children[1] );
	
	BOOST_TEST( c.data.atom == "c" );
	BOOST_TEST( c.data.indices.lower.size() == 0 );
	BOOST_TEST( c.data.indices.upper.size() == 0 );
	
	BOOST_TEST_REQUIRE( a_plus_b.children.size() == 2 );
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( a_plus_b.children[0] ) );
	auto a = cxxmath::get_node<indexed_string_atom>( a_plus_b.children[0] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( a_plus_b.children[1] ) );
	auto b = cxxmath::get_node<indexed_string_atom>( a_plus_b.children[1] );
	
	BOOST_TEST( a.data.atom == "a" );
	BOOST_TEST( a.data.indices.lower.size() == 0 );
	BOOST_TEST( a.data.indices.upper.size() == 0 );
	
	BOOST_TEST( b.data.atom == "b" );
	BOOST_TEST( b.data.indices.lower.size() == 0 );
	BOOST_TEST( b.data.indices.upper.size() == 0 );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<product>( d_times_e_over_f_plus_g.children[0] ) );
	auto d_times_e = cxxmath::get_node<product>( d_times_e_over_f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<sum>( d_times_e_over_f_plus_g.children[1] ) );
	auto f_plus_g = cxxmath::get_node<sum>( d_times_e_over_f_plus_g.children[1] );
	
	BOOST_TEST_REQUIRE( d_times_e.children.size() == 2 );
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( d_times_e.children[0] ) );
	auto d = cxxmath::get_node<indexed_string_atom>( d_times_e.children[0] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( d_times_e.children[1] ) );
	auto e = cxxmath::get_node<indexed_string_atom>( d_times_e.children[1] );
	
	BOOST_TEST( d.data.atom == "d" );
	BOOST_TEST( d.data.indices.lower.size() == 0 );
	BOOST_TEST( d.data.indices.upper.size() == 0 );
	
	BOOST_TEST( e.data.atom == "e" );
	BOOST_TEST( e.data.indices.lower.size() == 0 );
	BOOST_TEST( e.data.indices.upper.size() == 0 );
	
	BOOST_TEST_REQUIRE( f_plus_g.children.size() == 2 );
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( f_plus_g.children[0] ) );
	auto f = cxxmath::get_node<indexed_string_atom>( f_plus_g.children[0] );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_string_atom>( f_plus_g.children[1] ) );
	auto g = cxxmath::get_node<indexed_string_atom>( f_plus_g.children[1] );
	
	BOOST_TEST( f.data.atom == "f" );
	BOOST_TEST( f.data.indices.lower.size() == 0 );
	BOOST_TEST( f.data.indices.upper.size() == 0 );
	
	BOOST_TEST( g.data.atom == "g" );
	BOOST_TEST( g.data.indices.lower.size() == 0 );
	BOOST_TEST( g.data.indices.upper.size() == 0 );
}

BOOST_AUTO_TEST_CASE( parse_math_associative ) {
	using indexed_string_atom = atom_with_optional_indices<std::string>;
	
	std::string test = "a b c d e";
	auto root_incarnation = parsing::parse_math( test );
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<product>( root_incarnation ) );
	auto root_product = cxxmath::get_node<product>( root_incarnation );
	auto factors = cxxmath::children( root_product );
	
	parsing::math_tree tree_a = { indexed_string_atom{ "a", {} } };
	parsing::math_tree tree_b = { indexed_string_atom{ "b", {} } };
	parsing::math_tree tree_c = { indexed_string_atom{ "c", {} } };
	parsing::math_tree tree_d = { indexed_string_atom{ "d", {} } };
	parsing::math_tree tree_e = { indexed_string_atom{ "e", {} } };
	
	std::vector<parsing::math_tree> compare_factors = { tree_a, tree_b, tree_c, tree_d, tree_e };
	BOOST_TEST( compare_factors == factors );
}

BOOST_AUTO_TEST_CASE( parse_math_precedence ) {
	using indexed_string_atom = atom_with_optional_indices<std::string>;
	
	std::string test = "a / b c";
	auto root_incarnation = parsing::parse_math( test );
	
	parsing::math_tree tree_a = { indexed_string_atom{ "a", {} } };
	parsing::math_tree tree_b = { indexed_string_atom{ "b", {} } };
	parsing::math_tree tree_c = { indexed_string_atom{ "c", {} } };
	
	parsing::math_tree compare = {
		mathutils::product{},
		parsing::math_tree{
			mathutils::quotient{},
			tree_a,
			tree_b
		},
		tree_c
	};
	
	BOOST_TEST( compare == root_incarnation );
	
	test = "a - b c";
	root_incarnation = parsing::parse_math( test );
	
	compare = {
		mathutils::difference{},
		tree_a,
		parsing::math_tree{
			mathutils::product{},
			tree_b,
			tree_c
		},
	};
	
	BOOST_TEST( compare == root_incarnation );
}
