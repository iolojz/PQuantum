//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch - Free Polynomial

#include <boost/test/included/unit_test.hpp>

#include "tag_dispatch/tag_dispatch.hpp"

namespace tag_dispatch::models
{
template<class Coefficient, class Variable>
std::ostream &operator<<( std::ostream &os, const free_monomial<Coefficient, Variable> &m )
{
	os << m.coefficient;
	for( const auto &v : m.variables )
		os << " " << v;
	return os;
}

template<class Coefficient, class Variable, class CoefficientRing, class TotalVariablePreorder>
std::ostream &
operator<<( std::ostream &os, const free_polynomial<Coefficient, Variable, CoefficientRing, TotalVariablePreorder> &p )
{
	using monomial = typename std::decay_t<decltype( p )>::monomial;
	
	const auto &monomials = p.monomials();
	if( monomials.empty())
		return os << "0";
	
	auto it = monomials.begin();
	os << monomial{ it->second, it->first };
	for( ++it; it != monomials.end(); ++it )
		os << " + " << monomial{ it->second, it->first };
	return os;
}
}

BOOST_AUTO_TEST_CASE( free_polynomial )
{
	using namespace tag_dispatch;
	using polynomial_tag = models::free_polynomial_tag<int, std::string_view>;
	
	static constexpr auto make_monomial = make_free_monomial<int, std::string_view>;
	static constexpr auto make_polynomial = make_free_polynomial<int, std::string_view>;
	
	auto ab42 = make_polynomial( make_monomial( 42, "a", "b" ));
	auto ab84 = make_polynomial( make_monomial( 84, "a", "b" ));
	auto abab1764 = make_polynomial( make_monomial( 42 * 42, "a", "b", "a", "b" ));
	auto one_ = make_polynomial( make_monomial( 1 ));
	
	BOOST_TEST( make_polynomial( ab42 ) == ab42 );
	BOOST_TEST( multiply<>( ab42, ab42 ) == abab1764 );
	BOOST_TEST( add<>( ab42, ab42 ) == ab84 );
	BOOST_TEST( multiply<>( one<polynomial_tag>(), ab42 ) == ab42 );
	BOOST_TEST( multiply<>( ab42, one<polynomial_tag>()) == ab42 );
	BOOST_TEST( multiply<>( ab42, one<polynomial_tag>()) == ab42 );
	BOOST_TEST( one<polynomial_tag>() == one_ );
	
	BOOST_TEST( ab42 != one_ );
	BOOST_TEST( abab1764 != one_ );
}
