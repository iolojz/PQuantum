//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch - int

#include <boost/test/included/unit_test.hpp>

#include "tag_dispatch/tag_dispatch.hpp"

namespace tag_dispatch::impl {
template<>
struct less_equal<int, concepts::total_preorder<int, void>>
{
	static constexpr bool apply( int a, int b )
	{
		if( a < 0 )
			return true;
		if( b < 0 )
			return false;
		
		return (( a % 5 ) <= ( b % 5 ));
	}
};
}

namespace
{
using total_preorder = tag_dispatch::concepts::total_preorder<int, void>;
using total_order = tag_dispatch::concepts::total_order<int, void>;
}

BOOST_AUTO_TEST_CASE( total_order )
{
	using namespace tag_dispatch;
	
	int a = 23;
	int b = 42;
	int c = b;
	
	BOOST_TEST(less<total_order>(a, b));
	BOOST_TEST(less<total_order>(a, c));
	
	BOOST_TEST(less_equal<total_order>(a, b));
	BOOST_TEST(less_equal<total_order>(b, c));
	
	BOOST_TEST(greater_equal<total_order>(b, a));
	BOOST_TEST(greater_equal<total_order>(b, c));
	
	BOOST_TEST(greater<total_order>(b, a));
	BOOST_TEST(greater<total_order>(c, a));
	
	BOOST_TEST(equal<total_order>(b, c));
}

BOOST_AUTO_TEST_CASE( total_preorder )
{
	using namespace tag_dispatch;
	
	int a = -23;
	int b = 50;
	int c = 41;
	
	BOOST_TEST(less_equal<total_preorder>(a, a));
	BOOST_TEST(less_equal<total_preorder>(a, b));
	BOOST_TEST(less_equal<total_preorder>(b, c));
	BOOST_TEST(less_equal<total_preorder>(b, b));
	BOOST_TEST(less_equal<total_preorder>(b, c));
	BOOST_TEST(less_equal<total_preorder>(c, c));
	
	BOOST_TEST(greater<total_preorder>(c, b));
	BOOST_TEST(greater<total_preorder>(c, a));
	BOOST_TEST(greater<total_preorder>(b, a));
}

BOOST_AUTO_TEST_CASE(ring) {
	using namespace tag_dispatch;
	static_assert(concepts::ring<int>::value);
	
	int a = 42;
	int b = 33;
	int zero = tag_dispatch::zero<int>();
	int one = tag_dispatch::one<int>();
	
	BOOST_TEST(add<>(a, b) == 42 + 33);
	BOOST_TEST(subtract<>(a, b) == 42 - 33);
	BOOST_TEST(multiply<>(a, b) == 42 * 33);
	BOOST_TEST(zero == 0);
	BOOST_TEST(one == 1);
}
