//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE cxxmath - int

#include <boost/test/included/unit_test.hpp>

#include "cxxmath/cxxmath.hpp"

BOOST_AUTO_TEST_CASE( test_ring_int )
{
	using namespace cxxmath;
	using ring_int = default_ring_t<int>;
	
	BOOST_TEST( if_( ring_int::is_abelian_ring(), true, false ));
	
	BOOST_TEST( add( 42, 23 ) == 42 + 23 );
	BOOST_TEST( subtract( 42, 23 ) == 42 - 23 );
	BOOST_TEST( multiply( 42, 23 ) == 42 * 23 );
	BOOST_TEST( negate( 42 ) == -42 );
	
	BOOST_TEST( equal( 42, 42 ) == true );
	BOOST_TEST( equal( 42, 23 ) == false );
	
	BOOST_TEST( ring_int::zero() == 0 );
	BOOST_TEST( ring_int::one() == 1 );
	
	int a = 42;
	int b = 23;
	
	BOOST_TEST( add_assign( a, b ) == 42 + 23 );
	BOOST_TEST( a == 42 + 23 );
	
	BOOST_TEST( subtract_assign( a, b ) == 42 + 23 - 23 );
	BOOST_TEST( a == 42 + 23 - 23 );
	
	BOOST_TEST( multiply_assign( a, b ) == ( 42 + 23 - 23 ) * 23 );
	BOOST_TEST( a == ( 42 + 23 - 23 ) * 23 );
	
	BOOST_TEST( negate_in_place( a ) == -( 42 + 23 - 23 ) * 23 );
	BOOST_TEST( a == -( 42 + 23 - 23 ) * 23 );
}
