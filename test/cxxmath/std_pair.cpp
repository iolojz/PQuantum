//
// Created by jayz on 27.09.19.
//

#define BOOST_TEST_MODULE cxxmath - std_pair

#include <boost/test/included/unit_test.hpp>

#include "cxxmath/cxxmath.hpp"

BOOST_AUTO_TEST_CASE( test_product_std_pair )
{
	using int_pair = std::pair<int, int>;
	using namespace cxxmath;
	
	BOOST_TEST( first( int_pair{ 42, 23 } ) == 42 );
	BOOST_TEST( second( int_pair{ 42, 23 } ) == 23 );
	
	int_pair p{ 42, 23 };
	first( p ) = 0;
	second( p ) = 1;
	
	BOOST_TEST( first( p ) == 0 );
	BOOST_TEST( second( p ) == 1 );
}