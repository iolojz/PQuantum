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

BOOST_TEST_DONT_PRINT_LOG_VALUE( typename std::decay_t<decltype(std::declval<model::model_specification>().field_ids())>::iterator )

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
	BOOST_TEST( field_ids.size() == 3 );
	
	auto psi_id_it = std::find_if( std::begin( field_ids ), std::end( field_ids ), [&model] ( const auto &id ) {
		return model.field_specification_for_id( id ).name == "\\psi";
	} );
	auto bar_psi_id_it = std::find_if( std::begin( field_ids ), std::end( field_ids ), [&model] ( const auto &id ) {
		return model.field_specification_for_id( id ).name == "\\bar{\\psi}";
	} );
	auto a_id_it = std::find_if( std::begin( field_ids ), std::end( field_ids ), [&model] ( const auto &id ) {
		return model.field_specification_for_id( id ).name == "A";
	} );
	
	BOOST_TEST_REQUIRE(psi_id_it != std::end( field_ids ));
	BOOST_TEST_REQUIRE(bar_psi_id_it != std::end( field_ids ));
	BOOST_TEST_REQUIRE(a_id_it != std::end( field_ids ));
	
	support::uuid psi_id = *psi_id_it;
	support::uuid bar_psi_id = *bar_psi_id_it;
	support::uuid a_id = *a_id_it;
	
	auto psi_specification = model.field_specification_for_id( psi_id );
	auto a_specification = model.field_specification_for_id( a_id );
	auto bar_psi_specification = model.field_specification_for_id( bar_psi_id );
	
	BOOST_TEST( psi_specification.name == "\\psi" );
	BOOST_TEST(
	psi_specification.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann );
	BOOST_TEST( std::holds_alternative<int>( psi_specification.dimension ));
	BOOST_TEST( std::get<int>( psi_specification.dimension ) == 4 );
	
	BOOST_TEST( a_specification.name == "A" );
	BOOST_TEST( a_specification.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::real );
	BOOST_TEST( std::holds_alternative<mathutils::spacetime_dimension>( a_specification.dimension ));
	
	BOOST_TEST( bar_psi_specification.name == "\\bar{\\psi}" );
	BOOST_TEST(
			bar_psi_specification.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann );
	BOOST_TEST( std::holds_alternative<int>( bar_psi_specification.dimension ));
	BOOST_TEST( std::get<int>( bar_psi_specification.dimension ) == 4 );
	
	const auto &spacetime = model.spacetime();
	BOOST_TEST( std::holds_alternative<mathutils::manifold_types::vector_space>( spacetime ));
	
	const auto &spacetime_vector_space = std::get<mathutils::manifold_types::vector_space>( spacetime );
	BOOST_TEST( spacetime_vector_space.field == mathutils::manifold_types::vector_space::algebraic_field::real );
	BOOST_TEST( std::holds_alternative<mathutils::spacetime_dimension>( spacetime_vector_space.dimension ));
	BOOST_TEST(
	spacetime_vector_space.metric == mathutils::manifold_types::vector_space::vector_space_metric::euclidean );
	
	
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