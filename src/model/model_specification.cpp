//
// Created by jayz on 19.08.19.
//

#include "model_specification.hpp"

namespace {
using namespace PQuantum;
using namespace PQuantum::model;

struct to_lagrangian_impl {
	support::uuid invalid_id;
	
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
		auto is_derivative = [] ( const auto &ch_tch ) {
			if( cxxmath::holds_node<mathutils::spacetime_derivative>( boost::get<1>( ch_tch ) )
				|| cxxmath::holds_node<mathutils::dirac_operator>( boost::get<1>( ch_tch ) ) )
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
}

namespace PQuantum::model {
lagrangian_tree model_specification::to_lagrangian(
	boost::uuids::random_generator &uuid_gen,
	const input_lagrangian_tree &input
) {
	support::uuid invalid_id{ uuid_gen };
	// TODO: assert that the lagrangian does not contain invalid_id
	return cxxmath::recursive_tree_transform( input, to_lagrangian_impl{ invalid_id } );
}

model_specification::model_specification(
	std::string &&n, mathutils::manifold_specification &&m,
	boost::uuids::random_generator &&ug,
	std::map<support::uuid, classical_field_specification> &&fi,
	std::map<support::uuid, std::string> &&pi, input_lagrangian_tree &&il
) : model_name{std::move( n )}, spacetime_manifold{std::move( m )},
	uuid_gen{ std::move( ug ) },
	field_id_map{std::move( fi )}, parameter_id_map{std::move( pi )},
	input_lagrangian_{std::move( il )}, lagrangian{ to_lagrangian( uuid_gen, input_lagrangian_ ) } {
	// TODO: assert that the lagrangian only contains correct ids
	
	std::cout << lagrangian << std::endl;
}
}