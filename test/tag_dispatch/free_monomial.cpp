//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch - Free Monomial

#include <boost/test/included/unit_test.hpp>

#include "tag_dispatch/tag_dispatch.hpp"

namespace tag_dispatch {
namespace impl {
template<class Coefficient, class Variable>
struct equal<models::free_monomial_tag<Coefficient, Variable>, concepts::set<models::free_monomial_tag<Coefficient, Variable>, void>> {
	template<class Monomial1, class Monomial2>
	static auto apply(const Monomial1 &m1, const Monomial2 &m2) {
		if(tag_dispatch::equal<>(m1.coefficient, m2.coefficient) == false)
			return false;
		for(const auto &variable_pair : boost::range::combine(m1.variables, m2.variables)) {
			if(boost::get<0>(variable_pair) != boost::get<1>(variable_pair))
				return false;
		}
		
		return true;
	}
};
}

namespace models {
template<class Coefficient, class Variable>
bool operator==(const free_monomial<Coefficient, Variable> &m1, const free_monomial<Coefficient, Variable> &m2) {
	return equal<>(m1, m2);
}

template<class Coefficient, class Variable>
bool operator!=(const free_monomial<Coefficient, Variable> &m1, const free_monomial<Coefficient, Variable> &m2) {
	return not_equal<>(m1, m2);
}

template<class Coefficient, class Variable>
std::ostream &operator<<(std::ostream &os, const free_monomial<Coefficient, Variable> &m) {
	os << m.coefficient;
	for(const auto &v : m.variables)
		os << " " << v;
	return os;
}
}
}

BOOST_AUTO_TEST_CASE(free_monomial) {
	using namespace tag_dispatch;
	using tag = models::free_monomial_tag<int, std::string_view>;
	
	auto ab42 = make_free_monomial<int, std::string_view>(42, "a", "b");
	auto abab1764 = make_free_monomial<int, std::string_view>(42 * 42, "a", "b", "a", "b");
	auto one = make_free_monomial<int, std::string_view>(1);
	
	BOOST_TEST(compose<>(ab42, ab42) == abab1764);
	BOOST_TEST(compose<>(neutral_element<tag>(), ab42) == ab42);
	BOOST_TEST(compose<>(ab42, neutral_element<tag>()) == ab42);
	BOOST_TEST(compose<>(ab42, neutral_element<tag>()) == ab42);
	BOOST_TEST(neutral_element<tag>() == one);
	
	BOOST_TEST(ab42 != one);
	BOOST_TEST(abab1764 != one);
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
