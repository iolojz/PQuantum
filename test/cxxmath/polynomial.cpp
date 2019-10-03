//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch - Free Polynomial

#include <boost/test/included/unit_test.hpp>

#include "cxxmath/cxxmath.hpp"

namespace cxxmath
{
template<class Monomial>
void print_monomial( std::ostream &os, const Monomial &m );

template<class Polynomial, class = std::enable_if_t<is_polynomial_tag<tag_of_t<Polynomial>>::value>>
std::ostream &operator<<( std::ostream &os, const Polynomial &p )
{
	const auto &monomials = p.monomials();
	if( monomials.empty())
		return os << "0";
	
	auto it = monomials.begin();
	print_monomial( os, *it );
	for( ++it; it != monomials.end(); ++it ) {
		os << " + ";
		print_monomial( os, *it );
	}
	return os;
}

template<class Monomial>
void print_monomial( std::ostream &os, const Monomial &m )
{
	os << std::get<1>( m );
	for( const auto &v : std::get<0>( m ))
		os << " " << v;
}
}

BOOST_AUTO_TEST_CASE( test_polynomial )
{
	using namespace cxxmath;
	using polynomial_tag = polynomial_tag<int, std::string_view>;
	using polynomial_ring = default_ring_t<polynomial_tag>;
	
	auto ab42 = make_polynomial<polynomial_tag>( 42, "a", "b" );
	auto ab84 = make_polynomial<polynomial_tag>( 84, "a", "b" );
	auto abab1764 = make_polynomial<polynomial_tag>( 42 * 42, "a", "b", "a", "b" );
	auto one_ = make_polynomial<polynomial_tag>( 1 );
	
	BOOST_TEST( make_polynomial<polynomial_tag>( ab42 ) == ab42 );
	BOOST_TEST( multiply( ab42, ab42 ) == abab1764 );
	BOOST_TEST( add( ab42, ab42 ) == ab84 );
	BOOST_TEST( multiply( polynomial_ring::one(), ab42 ) == ab42 );
	BOOST_TEST( multiply( ab42, polynomial_ring::one()) == ab42 );
	BOOST_TEST( multiply( ab42, polynomial_ring::one()) == ab42 );
	BOOST_TEST( polynomial_ring::one() == one_ );
	
	BOOST_TEST( ab42 != one_ );
	BOOST_TEST( abab1764 != one_ );
}
