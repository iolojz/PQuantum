//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE cxxmath - bool

#include <boost/test/included/unit_test.hpp>

#include "cxxmath/cxxmath.hpp"

BOOST_AUTO_TEST_CASE( test_bool )
{
	using namespace cxxmath;
	
	static constexpr bool yes = true;
	static constexpr bool no = false;
	
	BOOST_TEST( and_( yes, yes ) == true );
	BOOST_TEST( and_( yes, no ) == false );
	BOOST_TEST( and_( no, yes ) == false );
	BOOST_TEST( and_( no, no ) == false );
	
	BOOST_TEST( or_( yes, yes ) == true );
	BOOST_TEST( or_( yes, no ) == true );
	BOOST_TEST( or_( no, yes ) == true );
	BOOST_TEST( or_( no, no ) == false );
	
	BOOST_TEST( not_( yes ) == false );
	BOOST_TEST( not_( no ) == true );
	
	BOOST_TEST( if_( yes, 42, 23 ) == 42 );
	BOOST_TEST( if_( no, 42, 23 ) == 23 );
	
	BOOST_TEST( equal( yes, yes ) == true );
	BOOST_TEST( equal( yes, no ) == false );
	BOOST_TEST( equal( no, yes ) == false );
	BOOST_TEST( equal( no, no ) == true );
	
	BOOST_TEST( not_equal( yes, yes ) == false );
	BOOST_TEST( not_equal( yes, no ) == true );
	BOOST_TEST( not_equal( no, yes ) == true );
	BOOST_TEST( not_equal( no, no ) == false );
}

BOOST_AUTO_TEST_CASE( test_std_bool_constant )
{
	using namespace cxxmath;
	
	static constexpr std::true_type yes;
	static constexpr std::false_type no;
	
	BOOST_TEST( and_( yes, yes ).value == true );
	BOOST_TEST( and_( yes, no ).value == false );
	BOOST_TEST( and_( no, yes ).value == false );
	BOOST_TEST( and_( no, no ).value == false );
	
	BOOST_TEST( or_( yes, yes ).value == true );
	BOOST_TEST( or_( yes, no ).value == true );
	BOOST_TEST( or_( no, yes ).value == true );
	BOOST_TEST( or_( no, no ).value == false );
	
	BOOST_TEST( not_( yes ).value == false );
	BOOST_TEST( not_( no ).value == true );
	
	BOOST_TEST( if_( yes, 42, 23 ) == 42 );
	BOOST_TEST( if_( no, 42, 23 ) == 23 );
	
	BOOST_TEST( equal( yes, yes ).value == true );
	BOOST_TEST( equal( yes, no ).value == false );
	BOOST_TEST( equal( no, yes ).value == false );
	BOOST_TEST( equal( no, no ).value == true );
	
	BOOST_TEST( not_equal( yes, yes ).value == false );
	BOOST_TEST( not_equal( yes, no ).value == true );
	BOOST_TEST( not_equal( no, yes ).value == true );
	BOOST_TEST( not_equal( no, no ).value == false );
}
