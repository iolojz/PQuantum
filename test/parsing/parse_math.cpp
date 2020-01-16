//
// Created by jayz on 16.01.20.
//

#include "config.hpp"
#include "logging/logging.hpp"
#include "mathutils/tree_expression/arithmetic.hpp"
#include "support/parsing.hpp"

#define BOOST_TEST_MODULE Parse Math

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;

BOOST_AUTO_TEST_CASE( parse_math ) {
	std::string test = "a + b - c^d * e / (f + g)";
	
	using tree_expression = support::tree::tree_node<mathutils::tree_expression::parentheses>;
	auto tree = support::parsing::parse<tree_expression>( test.begin(), test.end());
}