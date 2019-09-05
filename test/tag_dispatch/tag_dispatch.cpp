//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch

#include <boost/test/included/unit_test.hpp>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/sequence/io.hpp>

#include "tag_dispatch/make.hpp"
#include "tag_dispatch/std_tags.hpp"
#include "tag_dispatch/concepts/comparable.hpp"

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
}
