//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch

#include <boost/test/included/unit_test.hpp>

#include "tag_dispatch/tag_dispatch.hpp"
#include "tag_dispatch/concepts/boolean_lattice.hpp"
#include "tag_dispatch/concepts/set.hpp"
#include "tag_dispatch/concepts/total_order.hpp"
#include "tag_dispatch/concepts/total_preorder.hpp"
#include "tag_dispatch/concepts/ring.hpp"
#include "tag_dispatch/models/bool.hpp"
#include "tag_dispatch/models/int.hpp"

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
using int_total_preorder = tag_dispatch::concepts::total_preorder<int, void>;
using int_total_order = tag_dispatch::concepts::total_order<int, void>;
}

BOOST_AUTO_TEST_CASE(boolean_lattice_bool) {
	using namespace tag_dispatch;
	
	static constexpr bool yes = true;
	static constexpr bool no = false;
	
	BOOST_TEST(not_<>(equal<>(yes, no)));
	
	BOOST_TEST(and_<>(yes, yes));
	BOOST_TEST(not_<>(and_<>(yes, no)));
	BOOST_TEST(not_<>(and_<>(no, yes)));
	BOOST_TEST(not_<>(and_<>(no, no)));
	
	BOOST_TEST(or_<>(yes, yes));
	BOOST_TEST(or_<>(yes, no));
	BOOST_TEST(or_<>(no, yes));
	BOOST_TEST(not_<>(or_<>(no, no)));
	
	BOOST_TEST(equal<>(yes, yes));
	BOOST_TEST(not_<>(equal<>(yes, no)));
	BOOST_TEST(not_<>(equal<>(no, yes)));
	BOOST_TEST(equal<>(no, no));
}

BOOST_AUTO_TEST_CASE(boolean_lattice_std_bool_constant) {
	using namespace tag_dispatch;
	
	static constexpr std::true_type yes;
	static constexpr std::false_type no;
	
	static_assert(not_<>(equal<>(yes, no)).value);
	
	static_assert(and_<>(yes, yes).value);
	static_assert(not_<>(and_<>(yes, no)).value);
	static_assert(not_<>(and_<>(no, yes)).value);
	static_assert(not_<>(and_<>(no, no)).value);
	
	static_assert(or_<>(yes, yes).value);
	static_assert(or_<>(yes, no).value);
	static_assert(or_<>(no, yes).value);
	static_assert(not_<>(or_<>(no, no)).value);
	
	static_assert(equal<>(yes, yes).value);
	static_assert(not_<>(equal<>(yes, no)).value);
	static_assert(not_<>(equal<>(no, yes)).value);
	static_assert(equal<>(no, no).value);
}

BOOST_AUTO_TEST_CASE( total_order )
{
	using namespace tag_dispatch;
	static_assert(tag_dispatch::concepts::set<int, tag_dispatch::concepts::total_order<int, void>>::value,
				  "int does not model the 'set' concept.");
	
	int a = 23;
	int b = 42;
	int c = b;
	
	BOOST_TEST( less<int_total_order>( a, b ));
	BOOST_TEST( less<int_total_order>( a, c ));
	
	BOOST_TEST( less_equal<int_total_order>( a, b ));
	BOOST_TEST( less_equal<int_total_order>( b, c ));
	
	BOOST_TEST( greater_equal<int_total_order>( b, a ));
	BOOST_TEST( greater_equal<int_total_order>( b, c ));
	
	BOOST_TEST( greater<int_total_order>( b, a ));
	BOOST_TEST( greater<int_total_order>( c, a ));
	
	BOOST_TEST( equal<int_total_order>( b, c ));
}

BOOST_AUTO_TEST_CASE( total_preorder )
{
	using namespace tag_dispatch;
	
	int a = -23;
	int b = 50;
	int c = 41;
	
	BOOST_TEST( less_equal<int_total_preorder>( a, a ));
	BOOST_TEST( less_equal<int_total_preorder>( a, b ));
	BOOST_TEST( less_equal<int_total_preorder>( b, c ));
	BOOST_TEST( less_equal<int_total_preorder>( b, b ));
	BOOST_TEST( less_equal<int_total_preorder>( b, c ));
	BOOST_TEST( less_equal<int_total_preorder>( c, c ));
	
	BOOST_TEST( greater<int_total_preorder>( c, b ));
	BOOST_TEST( greater<int_total_preorder>( c, a ));
	BOOST_TEST( greater<int_total_preorder>( b, a ));
}

BOOST_AUTO_TEST_CASE(ring_int) {
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

/*

using namespace PQuantum::tag_dispatch;

namespace
{
struct test_type
{
	bool operator==( const test_type & ) const
	{ return true; }
};

std::ostream &operator<<(std::ostream &os, const test_type &) {
	return os << "\"test\"";
}

template<class ...Args>
std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &t) {
	return boost::fusion::operators::operator<<(os, t);
}

template<class ...Args>
std::ostream &operator<<(std::ostream &os, const std::pair<Args...> &t) {
	return boost::fusion::operators::operator<<(os, t);
}

template<class T, class Alloc>
std::ostream &operator<<(std::ostream &os, const std::vector<T, Alloc> &v) {
	os << "{";
	if(v.empty())
		return os << "}";
	
	auto next_last = v.end() - 1;
	for(auto it = v.begin(); it != next_last; ++it)
		os << *it << ", ";
	return os << *next_last << "}";
}
}

namespace PQuantum::tag_dispatch::impl {
template<>
struct less<std_vector_tag> {
	template<class V1, class V2>
	static decltype(auto) apply(V1 &&v1, V2 &&v2) {
		return std::forward<V1>(v1) < std::forward<V2>(v2);
	}
};
}

BOOST_AUTO_TEST_CASE(make_std_types)
{
	auto tuple = make_tuple( 42, 3.5, test_type{} );
	auto vector = make_vector(42, 35L, 44LL);
	auto pair = make_pair( 42, test_type{} );
	
	auto reference_tuple = std::make_tuple( 42, 3.5, test_type{} );
	std::vector<long long> reference_vector{42, 35, 44};
	auto reference_pair = std::make_pair( 42, test_type{} );
	
	static_assert(std::is_same_v<decltype(tuple), decltype(reference_tuple)>);
	static_assert(std::is_same_v<decltype(vector), decltype(reference_vector)>);
	static_assert(std::is_same_v<decltype(pair), decltype(reference_pair)>);
	
	BOOST_TEST( tuple == reference_tuple );
	BOOST_TEST( vector == reference_vector );
	BOOST_TEST( pair == reference_pair );
}

BOOST_AUTO_TEST_CASE(comparisons) {
	auto vector = make_vector(42, 35L, 44LL);
	equal(vector, vector);
}

*/
