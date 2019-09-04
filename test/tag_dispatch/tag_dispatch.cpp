//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch

#include <boost/test/included/unit_test.hpp>

#include "tag_dispatch/make.hpp"
#include "tag_dispatch/std_tags.hpp"
#include "tag_dispatch/comparisons.hpp"

using namespace PQuantum::tag_dispatch;

namespace
{
struct test_type
{
	bool operator==( const test_type & ) const
	{ return true; }
};
}

BOOST_AUTO_TEST_CASE( tag_dispatch )
{
	auto tuple = make_tuple( 42, 3.5, test_type{} );
	auto vector = make_vector( 42, 3.5 );
	auto pair = make_pair( 42, test_type{} );
	
	auto reference_tuple = std::make_tuple( 42, 3.5, test_type{} );
	std::vector<double> reference_vector{ 42, 3.5 };
	auto reference_pair = std::make_pair( 42, test_type{} );
	
	BOOST_TEST( tuple == reference_tuple );
	BOOST_TEST( vector == reference_vector );
	BOOST_TEST( pair == reference_pair );
}
