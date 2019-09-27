//
// Created by jayz on 27.09.19.
//

#define BOOST_TEST_MODULE cxxmath - product_monoid

#include <boost/test/included/unit_test.hpp>

#include "cxxmath/cxxmath.hpp"

namespace cxxmath
{
constexpr bool operator==( const product_monoid<int, int> &p1, const product_monoid<int, int> &p2 )
{
	return cxxmath::equal( p1, p2 );
}

std::ostream &operator<<( std::ostream &os, const product_monoid<int, int> &p )
{
	return os << "{" << first( p ) << ", " << second( p ) << "}";
}
}

BOOST_AUTO_TEST_CASE( test_product_monoid )
{
	using namespace cxxmath;
	using int_product = product_monoid<int, int>;
	
	using default_int_pair_monoid = default_monoid_t<tag_of_t<int_product>>;
	
	using first_monoid = typename default_group_t<int>::monoid_;
	using second_monoid = default_monoid_t<int>;
	using mixed_compose_assign = impl::compose_assign_product<first_monoid, second_monoid>;
	using mixed_neutral_element = impl::neutral_element_product<first_monoid, second_monoid>;
	using mixed_is_abelian = impl::is_abelian_monoid_product<first_monoid, second_monoid>;
	
	using mixed_int_pair_monoid = concepts::monoid<mixed_compose_assign, mixed_neutral_element, mixed_is_abelian, true>;
	
	auto pair0_1 = make_product_monoid( std::make_pair( 0, 1 ));
	auto pair1_1 = make_product_monoid( std::make_pair( 1, 1 ));
	auto pair42_23 = make_product_monoid( std::make_pair( 42, 23 ));
	auto pair1764_529 = make_product_monoid( std::make_pair( 42 * 42, 23 * 23 ));
	auto pair84_529 = make_product_monoid( std::make_pair( 42 + 42, 23 * 23 ));
	
	auto pair = pair42_23;
	
	BOOST_TEST( compose( pair42_23, pair42_23 ) == pair1764_529 );
	BOOST_TEST( default_int_pair_monoid::neutral_element() == pair1_1 );
	BOOST_TEST( default_int_pair_monoid::is_abelian_monoid() == true );
	BOOST_TEST( compose_assign( pair, pair ) == pair1764_529 );
	BOOST_TEST( pair == pair1764_529 );
	
	pair = pair42_23;
	
	BOOST_TEST( mixed_int_pair_monoid::compose( pair42_23, pair42_23 ) == pair84_529 );
	BOOST_TEST( mixed_int_pair_monoid::neutral_element() == pair0_1 );
	BOOST_TEST( mixed_int_pair_monoid::is_abelian_monoid() == true );
	BOOST_TEST( mixed_int_pair_monoid::compose_assign( pair, pair ) == pair84_529 );
	BOOST_TEST( pair == pair84_529 );
}