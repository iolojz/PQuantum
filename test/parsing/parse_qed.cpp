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

BOOST_TEST_DONT_PRINT_LOG_VALUE(
	typename std::decay_t<decltype( std::declval<model::model_specification>().field_ids() )>::iterator
)

BOOST_AUTO_TEST_CASE( parse_qed ) {
	logging::setup_logging_facilities();
	auto parser = parsing::qft_json_parser::parse( std::filesystem::path{PQUANTUM_EXAMPLES_DIRECTORY} / "qed.json" );
	
	auto json_header = parser.json_header();
	BOOST_TEST( json_header.name == "QFT JSON" );
	BOOST_TEST( json_header.version_major == 0 );
	BOOST_TEST( json_header.version_minor == 1 );
	
	auto model = parser.model();
	BOOST_TEST( model.name() == "qed" );
	
	auto field_ids = model.field_ids();
	BOOST_TEST( field_ids.size() == 3 );
	
	auto psi_id_it = std::find_if(
		std::begin( field_ids ), std::end( field_ids ), [&model]( const auto &id ) {
			return model.field_specification_for_id( id ).name == "\\psi";
		}
	);
	auto bar_psi_id_it = std::find_if(
		std::begin( field_ids ), std::end( field_ids ), [&model]( const auto &id ) {
			return model.field_specification_for_id( id ).name == "\\bar{\\psi}";
		}
	);
	auto a_id_it = std::find_if(
		std::begin( field_ids ), std::end( field_ids ), [&model]( const auto &id ) {
			return model.field_specification_for_id( id ).name == "A";
		}
	);
	
	BOOST_TEST_REQUIRE( psi_id_it != std::end( field_ids ) );
	BOOST_TEST_REQUIRE( bar_psi_id_it != std::end( field_ids ) );
	BOOST_TEST_REQUIRE( a_id_it != std::end( field_ids ) );
	
	support::uuid psi_id = *psi_id_it;
	support::uuid bar_psi_id = *bar_psi_id_it;
	support::uuid a_id = *a_id_it;
	
	auto psi_specification = model.field_specification_for_id( psi_id );
	auto a_specification = model.field_specification_for_id( a_id );
	auto bar_psi_specification = model.field_specification_for_id( bar_psi_id );
	
	BOOST_TEST( psi_specification.name == "\\psi" );
	BOOST_TEST(
		psi_specification.algebraic_field
			== mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann );
	BOOST_TEST( std::holds_alternative<mathutils::clifford_algebra_target_dimension>( psi_specification.dimension ) );
	
	BOOST_TEST( a_specification.name == "A" );
	BOOST_TEST( a_specification.algebraic_field == mathutils::manifold_types::vector_space::algebraic_field::real );
	BOOST_TEST( std::holds_alternative<mathutils::spacetime_dimension>( a_specification.dimension ) );
	
	BOOST_TEST( bar_psi_specification.name == "\\bar{\\psi}" );
	BOOST_TEST(
		bar_psi_specification.algebraic_field
			== mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann );
	BOOST_TEST(
		std::holds_alternative<mathutils::clifford_algebra_target_dimension>( bar_psi_specification.dimension ) );
	
	const auto &spacetime = model.spacetime();
	BOOST_TEST( std::holds_alternative<mathutils::manifold_types::vector_space>( spacetime ) );
	
	const auto &spacetime_vector_space = std::get<mathutils::manifold_types::vector_space>( spacetime );
	BOOST_TEST( spacetime_vector_space.field == mathutils::manifold_types::vector_space::algebraic_field::real );
	BOOST_TEST( std::holds_alternative<mathutils::spacetime_dimension>( spacetime_vector_space.dimension ) );
	BOOST_TEST(
		spacetime_vector_space.metric == mathutils::manifold_types::vector_space::vector_space_metric::euclidean );
	
	auto parameter_ids = model.parameter_ids();
	BOOST_TEST( parameter_ids.size() == 5 );
	
	auto zpsi_id_it = std::find_if(
		std::begin( parameter_ids ), std::end( parameter_ids ), [&model]( const auto &id ) {
			return model.parameter_for_id( id ).name == "Z_\\psi";
		}
	);
	auto m_id_it = std::find_if(
		std::begin( parameter_ids ), std::end( parameter_ids ), [&model]( const auto &id ) {
			return model.parameter_for_id( id ).name == "m";
		}
	);
	auto za_id_it = std::find_if(
		std::begin( parameter_ids ), std::end( parameter_ids ), [&model]( const auto &id ) {
			return model.parameter_for_id( id ).name == "Z_A";
		}
	);
	auto e_id_it = std::find_if(
		std::begin( parameter_ids ), std::end( parameter_ids ), [&model]( const auto &id ) {
			return model.parameter_for_id( id ).name == "e";
		}
	);
	auto xi_id_it = std::find_if(
		std::begin( parameter_ids ), std::end( parameter_ids ), [&model]( const auto &id ) {
			return model.parameter_for_id( id ).name == "\\xi";
		}
	);
	
	BOOST_TEST_REQUIRE( zpsi_id_it != std::end( parameter_ids ) );
	BOOST_TEST_REQUIRE( m_id_it != std::end( parameter_ids ) );
	BOOST_TEST_REQUIRE( za_id_it != std::end( parameter_ids ) );
	BOOST_TEST_REQUIRE( e_id_it != std::end( parameter_ids ) );
	BOOST_TEST_REQUIRE( xi_id_it != std::end( parameter_ids ) );
	
	support::uuid zpsi_id = *zpsi_id_it;
	support::uuid m_id = *m_id_it;
	support::uuid za_id = *za_id_it;
	support::uuid e_id = *e_id_it;
	support::uuid xi_id = *xi_id_it;
	
	auto lagrangian = model.model_lagrangian();
	BOOST_TEST_REQUIRE( cxxmath::holds_node<mathutils::sum>( lagrangian ) );
	auto lagrangian_sum_node = cxxmath::get_node<mathutils::sum>( lagrangian );
	BOOST_TEST_REQUIRE( std::size( lagrangian.children ) == 5 );
	
	lagrangian_tree compare_fermion_kinetic_term = {
		mathutils::product{},
		mathutils::number{ 0, 1 },
		indexed_parameter{ zpsi_id, {} },
		indexed_field{ bar_psi_id, {} },
		dirac_operator{},
		indexed_field{ psi_id, {} }
	};
	BOOST_TEST( lagrangian.children[0] == compare_fermion_kinetic_term );
	
	lagrangian_tree compare_fermion_mass_term = {
		mathutils::negation{},
		lagrangian_tree {
			mathutils::product{},
			mathutils::number{ 0, 1 },
			indexed_parameter{ m_id, {} },
			indexed_field{ bar_psi_id, {} },
			indexed_field{ psi_id, {} }
		}
	};
	BOOST_TEST( lagrangian.children[1] == compare_fermion_mass_term );
	
	auto photon_kinetic_term = lagrangian.children[2];
	BOOST_TEST_REQUIRE( cxxmath::holds_node<mathutils::product>( photon_kinetic_term ) );
	auto photon_kinetic_term_node = cxxmath::get_node<mathutils::product>( photon_kinetic_term );
	BOOST_TEST_REQUIRE( std::size( photon_kinetic_term_node.children ) == 4 );
	
	auto f_mu_nu_tree = photon_kinetic_term_node.children[2];
	BOOST_TEST_REQUIRE( cxxmath::holds_node<mathutils::difference>( f_mu_nu_tree ) );
	auto f_mu_nu_node = cxxmath::get_node<mathutils::difference>( f_mu_nu_tree );
	BOOST_TEST_REQUIRE( std::size( f_mu_nu_node.children ) == 2 );
	
	auto d_mu_a_nu_tree = f_mu_nu_node.children[0];
	BOOST_TEST_REQUIRE( cxxmath::holds_node<mathutils::product>( d_mu_a_nu_tree ) );
	auto d_mu_a_nu_node = cxxmath::get_node<mathutils::product>( d_mu_a_nu_tree );
	BOOST_TEST_REQUIRE( std::size( d_mu_a_nu_tree.children ) == 2 );
	
	auto d_mu_tree = d_mu_a_nu_tree.children[0];
	auto a_nu_tree = d_mu_a_nu_tree.children[1];
	
	BOOST_TEST_REQUIRE( cxxmath::holds_node<spacetime_derivative>( d_mu_tree ) );
	BOOST_TEST_REQUIRE( cxxmath::holds_node<indexed_field>( a_mu_tree ) );
	
	auto d_mu = cxxmath::data( cxxmath::get_node<spacetime_derivative>( d_mu_tree ) );
	auto a_nu = cxxmath::data( cxxmath::get_node<indexed_field>( a_nu_tree ) );
	
	auto lower_mu_index = d_mu.index;
	BOOST_TEST_REQUIRE( lower_mu_index.variance == spacetime_index::index_variance::lower );
	auto mu_index = lower_mu_index.id;
	
	spacetime_index upper_mu_index = { spacetime_index::index_variance::upper, mu_index };
	
	BOOST_TEST_REQUIRE( std::size( a_nu.indices.lower ) == 1 );
	BOOST_TEST_REQUIRE( std::size( a_nu.indices.upper ) == 0 );
	auto nu_index = a_nu.indices.lower.front();
	
	spacetime_index lower_nu_index = { spacetime_index::index_variance::lower, nu_index };
	spacetime_index upper_mu_index = { spacetime_index::index_variance::upper, mu_index };
	spacetime_index upper_nu_index = { spacetime_index::index_variance::upper, nu_index };
	
	decltype(a_nu.indices) lower_nu_index_spec = a_nu.indices;
	
	decltype(a_nu.indices) lower_mu_index_spec = lower_nu_index_spec;
	lower_mu_index_spec.indices.lower.front() = mu_index;
	
	decltype(a_nu.indices) upper_nu_index_spec = lower_nu_index_spec;
	std::swap( upper_nu_index_spec.indices.lower, upper_nu_index_spec.indices.upper );
	
	decltype(a_nu.indices) upper_mu_index_spec = lower_mu_index_spec;
	std::swap( upper_mu_index_spec.indices.lower, upper_mu_index_spec.indices.upper );
	
	lagrangian_tree compare_photon_kinetic_term = {
		mathutils::product{},
		lagrangian_tree {
			mathutils::quotient{},
			mathutils::number{ 1, 0 },
			mathutils::number{ 4, 0 }
		},
		indexed_parameter{ za_id, {} },
		lagrangian_tree{
			mathutils::difference{},
			lagrangian_tree{
				mathutils::product{},
				mathutils::spacetime_derivative{ lower_mu_index },
				indexed_field{ a_id, lower_nu_index_spec }
			},
			lagrangian_tree{
				mathutils::product{},
				mathutils::spacetime_derivative{ lower_nu_index },
				indexed_field{ a_id, lower_mu_index_spec }
			}
		},
		lagrangian_tree{
			mathutils::difference{},
			lagrangian_tree{
				mathutils::product{},
				mathutils::spacetime_derivative{ upper_mu_index },
				indexed_field{ a_id, upper_nu_index_spec }
			},
			lagrangian_tree{
				mathutils::product{},
				mathutils::spacetime_derivative{ upper_nu_index },
				indexed_field{ a_id, upper_mu_index_spec }
			}
		}
	};
	BOOST_TEST( lagrangian.children[2 == photon_kinetic_term );
	
	
}