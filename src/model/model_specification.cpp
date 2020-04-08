//
// Created by jayz on 19.08.19.
//

#include "model_specification.hpp"

#include "calculation/lagrangian_manipulations.hpp"

namespace {
using namespace PQuantum;
using namespace PQuantum::model;

struct to_lagrangian_impl {
	support::uuid invalid_id;
	mutable logging::severity_logger logger;
	
	to_lagrangian_impl( support::uuid invalid ) : invalid_id{ invalid } {}
	
	template<
	    class Atom,
	    class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, mathutils::spacetime_derivative>>,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, mathutils::dirac_operator>>,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, indexed_field>>
	> lagrangian_tree operator()( Atom &&atom ) const {
		return std::forward<Atom>( atom );
	}
	
	lagrangian_tree operator()( const mathutils::spacetime_derivative & ) const {
		return indexed_parameter{ invalid_id, {} };
	}
	
	lagrangian_tree operator()( const mathutils::dirac_operator & ) const {
		return indexed_parameter{ invalid_id, {} };
	}
	
	lagrangian_tree operator()( const indexed_field &ifield ) const {
		return { mathutils::field_multiplication_operator{}, ifield };
	}
	
	template<
	    class NonTerminalNodeData,
	    class Children,
	    class TransformedChildren,
	    class = std::enable_if_t<!std::is_same_v<std::decay_t<NonTerminalNodeData>, mathutils::product>>,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<NonTerminalNodeData>, mathutils::parentheses>>
	> lagrangian_tree operator()( NonTerminalNodeData &&nt, Children &&, TransformedChildren &&tch ) const {
		return {
			std::forward<NonTerminalNodeData>( nt ),
			std::forward<TransformedChildren>( tch )
		};
	}
	
	template<class Children, class TransformedChildren>
	lagrangian_tree operator()( mathutils::product, Children &&ch, TransformedChildren &&tch ) const {
		BOOST_LOG_NAMED_SCOPE( "to_lagrangian_impl::operator()" );
		auto is_derivative = [] ( const auto &ch_tch ) {
			if( cxxmath::holds_node<mathutils::spacetime_derivative>( boost::get<0>( ch_tch ) )
				|| cxxmath::holds_node<mathutils::dirac_operator>( boost::get<0>( ch_tch ) ) )
				return true;
			
			return false;
		};
		
		auto combined = boost::range::combine( ch, tch );
		auto derivative_it = std::find_if( std::begin( combined ), std::end( combined ), is_derivative );
		
		std::vector<lagrangian_tree> factors( std::begin( tch ), boost::get<1>( derivative_it.get_iterator_tuple() ) );
		
		if( derivative_it != std::end( combined ) ) {
			const auto &derivative_node = boost::get<0>( *derivative_it );
			
			auto next = std::next( derivative_it );
			auto ch_tail = boost::make_iterator_range( boost::get<0>( next.get_iterator_tuple() ), std::end( ch ) );
			auto tch_tail = boost::make_iterator_range( boost::get<1>( next.get_iterator_tuple() ), std::end( tch ) );
			
			if( std::size( tch_tail ) == 0 ) {
				BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Derivative does not act on anything.";
				error::exit_upon_error();
			}
			
			if( cxxmath::holds_node<mathutils::spacetime_derivative>( derivative_node ) ) {
				factors.emplace_back(
					cxxmath::get_node<mathutils::spacetime_derivative>( derivative_node ).data,
					this->operator()( mathutils::product{}, ch_tail, tch_tail )
				);
			} else if( cxxmath::holds_node<mathutils::dirac_operator>( derivative_node ) ) {
				factors.emplace_back(
					mathutils::dirac_operator{},
					this->operator()( mathutils::product{}, ch_tail, tch_tail )
				);
			}
		}
		
		return { mathutils::product{}, std::move( factors ) };
	}
	
	template<class Children, class TransformedChildren>
	lagrangian_tree operator()( mathutils::parentheses, Children &&, TransformedChildren &&tch ) const {
		return std::forward<TransformedChildren>( tch ).front();
	}
};

struct error_if_invalid_impl {
	support::uuid invalid_id;
	mutable logging::severity_logger logger;
	
	error_if_invalid_impl( support::uuid invalid ) : invalid_id{ invalid } {}
	
	template<class Node> void operator()( const Node &node ) const {
		using tree_node = cxxmath::default_tree_node_t<cxxmath::tag_of_t<decltype(node)>>;
		
		if constexpr( std::decay_t<decltype(tree_node::is_terminal( node ) )>{} ) {
			if constexpr( std::is_same_v<std::decay_t<decltype(node.data)>, model::indexed_parameter> ) {
				if( node.data.id == invalid_id ) {
					BOOST_LOG_SEV( logger, logging::severity_level::error )
						<< "Derivative occuring outside of product.";
					error::exit_upon_error();
				}
			}
		} else {
			const auto &children = tree_node::children( node );
			for( const auto &child : children )
				cxxmath::visit( *this, child );
		}
	}
};

void error_if_invalid( const model::lagrangian_tree &lagrangian, support::uuid invalid_id ) {
	cxxmath::visit( error_if_invalid_impl{ invalid_id }, lagrangian );
}
}

namespace PQuantum::model {
lagrangian_tree model_specification::to_lagrangian(
	boost::uuids::random_generator &uuid_gen,
	const input_lagrangian_tree &input
) {
	logging::severity_logger logger;
	BOOST_LOG_NAMED_SCOPE( "model_specification::to_lagrangian()" );
	
	support::uuid invalid_id{ uuid_gen };
	auto lagrangian = cxxmath::recursive_tree_transform( input, to_lagrangian_impl{ invalid_id } );
	error_if_invalid( lagrangian, invalid_id );
	
	return calculation::simplify_arithmetic( lagrangian );
}

model_specification::model_specification(
	std::string &&n, mathutils::manifold_specification &&m,
	boost::uuids::random_generator &&ug,
	std::map<support::uuid, classical_field_specification> &&fi,
	std::map<support::uuid, std::string> &&pi, input_lagrangian_tree &&il
) : model_name{std::move( n )}, spacetime_manifold{std::move( m )},
	uuid_gen{ std::move( ug ) },
	field_id_map{std::move( fi )}, parameter_id_map{std::move( pi )},
	input_lagrangian_{std::move( il )}, lagrangian_{ to_lagrangian( uuid_gen, input_lagrangian_ ) } {
	BOOST_LOG_NAMED_SCOPE( "model_specification::model_specification()" );
	// TODO: assert that the lagrangian only contains correct ids
}
}