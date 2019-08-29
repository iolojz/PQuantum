//
// Created by jayz on 28.08.19.
//

#define BOOST_TEST_MODULE Clifford Algebra

#include <boost/test/included/unit_test.hpp>

#include "io/logging.hpp"
#include "mathutils/linear_operator.hpp"
#include "mathutils/abstract_algebra/clifford.hpp"

using namespace PQuantum;

namespace {
struct euclidean_form_int {
	int operator()(const mathutils::gamma_matrix &g1, const mathutils::gamma_matrix &g2) {
		BOOST_LOG_NAMED_SCOPE("euclidean_form_int::operator()()");
		io::severity_logger logger;
		
		if(!std::holds_alternative<int>(g1.index) || !std::holds_alternative<int>(g2.index)) {
			BOOST_LOG_SEV(logger, io::severity_level::internal_error)
				<< "Non-integer indices of gamma matrices are not supported.";
		}
		
		if(std::get<int>(g1.index) == std::get<int>(g2.index))
			return 1;
		
		return 0;
	}
};

using clifford_int = typename mathutils::clifford_quotient<mathutils::gamma_matrix, int, euclidean_form_int>::clifford;
}

BOOST_AUTO_TEST_CASE(clifford_algebra) {
	BOOST_TEST(true);
}
