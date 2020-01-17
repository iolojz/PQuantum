//
// Created by jayz on 16.01.20.
//

#define BOOST_SPIRIT_X3_DEBUG

#include "config.hpp"
#include "logging/logging.hpp"
#include "mathutils/tree_expression/arithmetic.hpp"
#include "support/parsing.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;

BOOST_AUTO_TEST_CASE( parse_math ) {
	std::string test = "a + b - c^d * e / (f + g)";
	
	using tree_expression = mathutils::tree_expression::arithmetic_tree;
	auto result = support::parsing::parse<tree_expression>( test.begin(), test.end());
	
	BOOST_TEST_REQUIRE( result.index() == 1, "test string could not be parsed" );
	
	auto root_incarnation = std::get<0>( result ).node_incarnation;
}