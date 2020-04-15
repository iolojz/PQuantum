#include "mathutils/number.hpp"

#define BOOST_TEST_MODULE Number

#include <boost/test/included/unit_test.hpp>

using namespace PQuantum;

namespace {
std::pair<boost::multiprecision::cpp_rational, boost::multiprecision::cpp_rational>
test_multiplication(
	const boost::multiprecision::cpp_rational &real1,
	const boost::multiprecision::cpp_rational &imag1,
	const boost::multiprecision::cpp_rational &real2,
	const boost::multiprecision::cpp_rational &imag2
) {
	boost::multiprecision::cpp_rational real = real1;
	boost::multiprecision::cpp_rational imag = imag1;
	
	boost::multiprecision::cpp_rational new_real = real * real2 - imag * imag2;
	imag *= real2;
	real *= imag2;
	
	imag += std::move( real );
	real = std::move( new_real );
	
	return std::make_pair( std::move( real ), std::move( imag ) );
}
}

BOOST_AUTO_TEST_CASE( boost_cpp_stuff ) {
	boost::multiprecision::cpp_rational zero = 0;
	boost::multiprecision::cpp_rational one = 1;
	
	boost::multiprecision::cpp_rational real = 0;
	boost::multiprecision::cpp_rational imag = 1;
	
	boost::multiprecision::cpp_rational other_real = 1;
	boost::multiprecision::cpp_rational other_imag = 0;
	
	auto pair = test_multiplication( real, imag, other_real, other_imag );
	real = std::move( pair.first );
	imag = std::move( pair.second );
	
	BOOST_TEST( real == 0 );
	BOOST_TEST( imag == 1 );
}

BOOST_AUTO_TEST_CASE( number ) {
	mathutils::number zero = 0;
	mathutils::number one = 1;
	mathutils::number i( 0, 1 );
	
	BOOST_TEST( zero * one == zero );
	BOOST_TEST( zero * i == zero );
	BOOST_TEST( zero * zero == zero );
	
	BOOST_TEST( i * one == i );
	BOOST_TEST( one * i == i );
}
