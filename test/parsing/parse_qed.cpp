//
// Created by jayz on 19.08.19.
//

#include "config.hpp"
#include "logging/logging.hpp"
#include "parsing/qft_json_parser.hpp"

#define BOOST_TEST_MODULE Parse QED

#include <boost/test/included/unit_test.hpp>

#include <filesystem>

using namespace PQuantum;

BOOST_AUTO_TEST_CASE(parse_qed) {
	logging::setup_logging_facilities();
	auto parser = parsing::qft_json_parser::parse(std::filesystem::path{PQUANTUM_EXAMPLES_DIRECTORY} / "qed.json");
	
	auto json_header = parser.json_header();
	BOOST_TEST(json_header.name == "QFT JSON");
	BOOST_TEST(json_header.version_major == 0);
	BOOST_TEST(json_header.version_minor == 1);
	
	auto model = parser.model();
	BOOST_TEST(model.name() == "qed");
	
	auto field_ids = model.field_ids();
	BOOST_TEST( field_ids.size() == 2 );
	
	auto psi_id = field_ids.front();
	auto psi_specification = model.field_specification_for_id( field_ids.front());
	
	auto a_id = field_ids.front();
	auto a_specification = model.field_specification_for_id( field_ids.back());
	
	if( psi_specification.name == "A" && a_specification.name == "\\psi" ) {
		std::swap(psi_id, a_id);
		std::swap(psi_specification, a_specification);
	}
	
	BOOST_TEST( psi_specification.name == "\\psi" );
	BOOST_TEST(
	psi_specification.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann );
	BOOST_TEST( std::holds_alternative<int>( psi_specification.dimension ));
	BOOST_TEST( std::get<int>( psi_specification.dimension ) == 4 );
	
	BOOST_TEST( a_specification.name == "A" );
	BOOST_TEST( a_specification.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::real );
	BOOST_TEST( std::holds_alternative<mathutils::spacetime_dimension>( a_specification.dimension ));
	
	const auto &spacetime = model.spacetime();
	BOOST_TEST( std::holds_alternative<mathutils::manifold_types::vector_space>( spacetime ));
	
	const auto &spacetime_vector_space = std::get<mathutils::manifold_types::vector_space>( spacetime );
	BOOST_TEST( spacetime_vector_space.field == mathutils::manifold_types::vector_space::algebraic_field::real );
	BOOST_TEST( std::holds_alternative<mathutils::spacetime_dimension>( spacetime_vector_space.dimension ));
	BOOST_TEST(
	spacetime_vector_space.metric == mathutils::manifold_types::vector_space::vector_space_metric::euclidean );
	
	const auto &lagrangian_monomial_map = model.model_lagrangian().monomials();
	BOOST_TEST(lagrangian_monomial_map.size() == 5);
	
	/*
	const auto &kinetic_coefficient = lagrangian_monomial_map.at( { psi_id, mathutils::dirac_operator{}, psi_id } );

BOOST_TEST( kinetic_coefficient == mathutils::polynomial_expression::monomial{{}, number{}} );
	
	"monomial": "\\bar{\\psi} \\DiracOperator \\psi",
			"coefficient": "Z_\\psi",
			"constant factor": "i"
},
{
"monomial": "\\bar{\\psi} \\psi",
"coefficient": "m",
"constant factor": "-i"
},
{
"monomial": "(\\partial_\\mu A_\\nu - \\partial_\\nu A_\\mu) (\\partial^\\mu A^\\nu - \\partial^\\nu A^\\mu)",
"coefficient": "Z_A",
"constant factor": "1/4"
},
{
"monomial": "\\bar{\\psi} \\gamma_\\mu A^\\mu \\psi",
"coefficient": "e",
"constant factor": "-1"
},
{
"monomial": "(\\partial_\\mu A^\\mu)^2",
"coefficient": "xi"
	 */
}