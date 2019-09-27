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
