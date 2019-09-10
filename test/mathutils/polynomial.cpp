//
// Created by jayz on 28.08.19.
//

#define BOOST_TEST_MODULE Polynomial

#include <boost/test/included/unit_test.hpp>

#include "io/logging.hpp"
#include "tag_dispatch/models/free_polynomial.hpp"

using namespace PQuantum;

namespace
{
template<class Coefficient> using string_polynomial = mathutils::free_polynomial<std::string, Coefficient>;
}

BOOST_AUTO_TEST_CASE( polynomial_int )
{
	using polynomial = string_polynomial<int>;
	using monomial = typename polynomial::monomial;
	
	const auto zero = mathutils::abstract_algebra::ring<polynomial>::zero();
	const auto one = mathutils::abstract_algebra::ring<polynomial>::one();
	const auto two = one + one;
	
	const auto a = polynomial{ monomial{{ "a" }, 1 }};
	const auto b = polynomial{ monomial{{ "b" }, 1 }};
	const auto aa = polynomial{ monomial{{ "a", "a" }, 1 }};
	const auto ab = polynomial{ monomial{{ "a", "b" }, 1 }};
	const auto ba = polynomial{ monomial{{ "b", "a" }, 1 }};
	const auto bb = polynomial{ monomial{{ "b", "b" }, 1 }};
	
	BOOST_TEST( a != one );
	BOOST_TEST( aa != a );
	BOOST_TEST( aa != ab );
	BOOST_TEST( aa != bb );
	
	BOOST_TEST( zero == -zero );
	BOOST_TEST( a != -a );
	
	std::cout << a << " and " << -a << std::endl;
	
	BOOST_TEST( a * a == aa );
	BOOST_TEST( a * b == ab );
	BOOST_TEST( b * a == ba );
	BOOST_TEST( b * b == bb );
	
	BOOST_TEST( a * a * a == aa * a );
	BOOST_TEST( a * b * a == ab * a );
	BOOST_TEST( a * b * a == a * ba );
	
	BOOST_TEST( a + a == two * a );
	BOOST_TEST( b + b == b * two );
	BOOST_TEST( bb + bb == bb * two );
	BOOST_TEST(( b + a ) * a == ba + aa );
	
	BOOST_TEST( b - b == zero );
	BOOST_TEST( bb - bb == zero );
	
	BOOST_TEST(( a + b ) * ( a + b ) == aa + ab + ba + bb );
	BOOST_TEST(( a - b ) * ( a - b ) == aa - ab - ba + bb );
	BOOST_TEST(( a + b ) * ( a - b ) == aa - ab + ba - bb );
}
