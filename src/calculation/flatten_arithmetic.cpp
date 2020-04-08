//
// Created by jayz on 07.04.20.
//

#include "lagrangian_manipulations.hpp"

namespace {
using namespace PQuantum;

struct flatten_arithmetic_impl {
	template<class Atom>
	model::lagrangian_tree operator()( Atom &&atom ) const {
		return std::forward<Atom>( atom );
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree
	operator()( const mathutils::spacetime_derivative &sd, Children &&, TransformedChildren &&tch ) const {
		return { sd, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::dirac_operator, Children &&, TransformedChildren &&tch ) const {
		return { mathutils::dirac_operator{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree
	operator()( mathutils::field_multiplication_operator, Children &&, TransformedChildren &&tch ) const {
		return { mathutils::field_multiplication_operator{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree
	operator()( const mathutils::function_call<std::string> &fc, Children &&, TransformedChildren &&tch ) const {
		return { fc, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::sum, Children &&, TransformedChildren &&tch ) const {
		if( std::size( tch ) == 1 )
			return std::move( *std::begin( tch ) );
		
		std::vector<model::lagrangian_tree> summands;
		for( auto &&t : tch ) {
			if( cxxmath::holds_node<mathutils::sum>( t ) ) {
				auto &&children = cxxmath::get_node<mathutils::sum>( t ).children;
				summands.insert(
					std::end( summands ),
					std::make_move_iterator( std::begin( children ) ),
					std::make_move_iterator( std::end( children ) )
				);
			} else
				summands.push_back( std::move( t ) );
		}
		
		return { mathutils::sum{}, std::move( summands ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::product, Children &&, TransformedChildren &&tch ) const {
		if( std::size( tch ) == 1 )
			return std::move( *std::begin( tch ) );
		
		std::vector<model::lagrangian_tree> factors;
		for( auto &&t : tch ) {
			if( cxxmath::holds_node<mathutils::product>( t ) ) {
				auto &&children = cxxmath::get_node<mathutils::product>( t ).children;
				factors.insert(
					std::end( factors ),
					std::make_move_iterator( std::begin( children ) ),
					std::make_move_iterator( std::end( children ) )
				);
			} else
				factors.push_back( std::move( t ) );
		}
		
		return { mathutils::product{}, std::move( factors ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::difference, Children &&, TransformedChildren &&tch ) const {
		return { mathutils::difference{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::quotient, Children &&, TransformedChildren &&tch ) const {
		return { mathutils::quotient{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::power, Children &&, TransformedChildren &&tch ) const {
		return { mathutils::power{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::negation, Children &&, TransformedChildren &&tch ) const {
		if( cxxmath::holds_node<mathutils::negation>( *std::begin( tch ) ) ) {
			return cxxmath::get_node<mathutils::negation>( *std::begin( tch ) ).children.front();
		}
		
		return { mathutils::negation{}, std::forward<TransformedChildren>( tch ) };
	}
};
}

namespace PQuantum::calculation {
model::lagrangian_tree flatten_arithmetic( const model::lagrangian_tree &lagrangian ) {
	return cxxmath::recursive_tree_transform( lagrangian, flatten_arithmetic_impl{} );
}
}