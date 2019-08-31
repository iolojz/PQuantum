//
// Created by jayz on 28.08.19.
//

#define BOOST_TEST_MODULE Clifford Algebra

#include <boost/test/included/unit_test.hpp>

#include "io/logging.hpp"
#include "mathutils/linear_operators.hpp"
#include "mathutils/abstract_algebra/clifford.hpp"

using namespace PQuantum;

namespace {
struct euclidean_form_int {
	int operator()(const mathutils::gamma_matrix &g1, const mathutils::gamma_matrix &g2) {
		BOOST_LOG_NAMED_SCOPE("euclidean_form_int::operator()()");
		io::severity_logger logger;
		
		if( !std::holds_alternative<int>( g1.index.id ) || !std::holds_alternative<int>( g2.index.id )) {
			BOOST_LOG_SEV(logger, io::severity_level::internal_error)
				<< "Non-integer indices of gamma matrices are not supported.";
		}
		
		if( std::get<int>( g1.index.id ) == std::get<int>( g2.index.id ))
			return 1;
		
		return 0;
	}
};

using clifford_int = typename mathutils::clifford_quotient<mathutils::gamma_matrix, int, euclidean_form_int>::type;
}

BOOST_AUTO_TEST_CASE( clifford_algebra_int )
{
	static constexpr auto lower = mathutils::spacetime_index::index_variance::lower;
	using polynomial = typename clifford_int::underlying_polynomial;
	using monomial = typename polynomial::monomial;
	
	const auto gamma_1_symbol = mathutils::gamma_matrix{{ lower, 1 }};
	const auto gamma_2_symbol = mathutils::gamma_matrix{{ lower, 2 }};
	const auto gamma_3_symbol = mathutils::gamma_matrix{{ lower, 3 }};
	const auto gamma_4_symbol = mathutils::gamma_matrix{{ lower, 4 }};
	
	const auto one = mathutils::abstract_algebra::ring<clifford_int>::one();
	
	const auto gamma_1 = clifford_int{ polynomial{ monomial{{ gamma_1_symbol }, 1 }}};
	const auto gamma_2 = clifford_int{ polynomial{ monomial{{ gamma_2_symbol }, 1 }}};
	const auto gamma_3 = clifford_int{ polynomial{ monomial{{ gamma_3_symbol }, 1 }}};
	const auto gamma_4 = clifford_int{ polynomial{ monomial{{ gamma_4_symbol }, 1 }}};
	
	BOOST_TEST( gamma_1 * gamma_1 == one );
	BOOST_TEST( gamma_2 * gamma_2 == one );
	BOOST_TEST( gamma_3 * gamma_3 == one );
	
	BOOST_TEST( gamma_1 * gamma_2 != -gamma_1 * gamma_2 );
	BOOST_TEST( gamma_1 * gamma_2 != gamma_2 * gamma_1 );
	
	BOOST_TEST( gamma_1 * gamma_2 == -gamma_2 * gamma_1 );
	BOOST_TEST( gamma_1 * gamma_4 == -gamma_4 * gamma_1 );
	
	BOOST_TEST( gamma_1 * gamma_2 * gamma_3 == gamma_2 * gamma_3 * gamma_1 );
	BOOST_TEST( gamma_2 * gamma_1 * gamma_3 == -gamma_2 * gamma_3 * gamma_1 );
	BOOST_TEST( gamma_1 * gamma_2 * gamma_1 == -gamma_2 );
}
