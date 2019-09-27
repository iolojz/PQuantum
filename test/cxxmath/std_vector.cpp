//
// Created by jayz on 27.09.19.
//

#define BOOST_TEST_MODULE cxxmath - std::vector

#include <boost/test/included/unit_test.hpp>

#include "cxxmath/cxxmath.hpp"

BOOST_AUTO_TEST_CASE( test_monoid_std_vector )
{
	using namespace cxxmath;
	using monoid_std_vector = default_monoid_t<tag_of_t<std::vector<int>>>;
	
	std::vector<int> v42_23 = { 42, 23 };
	std::vector<int> v42_23_42_23 = { 42, 23, 42, 23 };
	std::vector<int> vempty;
	
	BOOST_TEST( if_( monoid_std_vector::is_abelian_monoid(), true, false ) == false );
	
	BOOST_TEST( monoid_std_vector::neutral_element() == vempty );
	
	BOOST_TEST( compose( vempty, v42_23 ) == v42_23 );
	BOOST_TEST( compose( v42_23, vempty ) == v42_23 );
	BOOST_TEST( compose( v42_23, v42_23 ) == v42_23_42_23 );
	
	auto v = vempty;
	
	BOOST_TEST( compose_assign( v, vempty ) == vempty );
	BOOST_TEST( v == vempty );
	BOOST_TEST( compose_assign( v, v42_23 ) == v42_23 );
	BOOST_TEST( v == v42_23 );
	BOOST_TEST( compose_assign( v, v42_23 ) == v42_23_42_23 );
	BOOST_TEST( v == v42_23_42_23 );
}