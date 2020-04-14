//
// Created by jayz on 07.04.20.
//

#include "lagrangian_manipulations.hpp"

#include <numeric>

namespace {
using namespace PQuantum;

struct simplify_arithmetic_impl {
	mutable logging::severity_logger logger;
	
	template<class Atom>
	model::lagrangian_tree operator()( Atom &&atom ) const {
		return std::forward<Atom>( atom );
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree
	operator()( const mathutils::spacetime_derivative &sd, Children &&, TransformedChildren &&tch ) const {
		auto &&transformed = tch.front();
		if( cxxmath::holds_node<mathutils::number>( transformed ) )
			return mathutils::number{ 0, 0 };
		
		return { sd, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::dirac_operator, Children &&, TransformedChildren &&tch ) const {
		auto &&transformed = tch.front();
		if( cxxmath::holds_node<mathutils::number>( transformed ) )
			return mathutils::number{ 0, 0 };
		
		return { mathutils::dirac_operator{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree
	operator()( mathutils::field_multiplication_operator, Children &&, TransformedChildren &&tch ) const {
		return { mathutils::field_multiplication_operator{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree
	operator()( const mathutils::index_sum &is, Children &&, TransformedChildren &&tch ) const {
		return { is, std::forward<TransformedChildren>( tch ) };
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
		
		auto numbers_end = std::stable_partition( std::begin( summands ), std::end( summands ),
			[] ( const auto &summand ) {
				return cxxmath::holds_node<mathutils::number>( summand );
			}
		);
		auto numerical_factor = cxxmath::get_node<mathutils::number>( std::reduce(
			std::make_move_iterator( std::begin( summands ) ),
			std::make_move_iterator( numbers_end ),
			model::lagrangian_tree{ mathutils::number{ 0, 0 } },
			[] ( auto &&num1, auto &&num2 ) -> model::lagrangian_tree {
				return (
					cxxmath::get_node<mathutils::number>( num1 ).data +
					cxxmath::get_node<mathutils::number>( num2 ).data
				);
			}
		) ).data;
		
		if( numerical_factor == mathutils::number{ 0, 0 } )
			summands.erase( std::begin( summands ), numbers_end );
		else {
			summands.erase( ++std::begin( summands ), numbers_end );
			summands.front() = std::move( numerical_factor );
		}
		
		if( std::empty( summands ) )
			return mathutils::number{ 0, 0 };
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
		
		auto numbers_end = std::stable_partition( std::begin( factors ), std::end( factors ),
			[] ( const auto &factor ) {
				return cxxmath::holds_node<mathutils::number>( factor );
			}
		);
		auto numerical_factor = cxxmath::get_node<mathutils::number>( std::reduce(
			std::make_move_iterator( std::begin( factors ) ),
			std::make_move_iterator( numbers_end ),
			model::lagrangian_tree{ mathutils::number{ 1, 0 } },
			[] ( auto &&num1, auto &&num2 ) -> model::lagrangian_tree {
				return (
					cxxmath::get_node<mathutils::number>( num1 ).data *
					cxxmath::get_node<mathutils::number>( num2 ).data
				);
			}
		) ).data;
		
		if( numerical_factor == mathutils::number{ 0, 0 } )
			return mathutils::number{ 0, 0 };
		
		if( numerical_factor == mathutils::number{ 1, 0 } )
			factors.erase( std::begin( factors ), numbers_end );
		else {
			factors.erase( ++std::begin( factors ), numbers_end );
			factors.front() = std::move( numerical_factor );
		}
		
		if( std::empty( factors ) )
			return mathutils::number{ 1, 0 };
		return { mathutils::product{}, std::move( factors ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::difference, Children &&ch, TransformedChildren &&tch ) const {
		if( cxxmath::holds_node<mathutils::number>( tch.front() ) ) {
			auto &&minuend = cxxmath::get_node<mathutils::number>( tch.front() );
			
			if( cxxmath::holds_node<mathutils::number>( tch.back() ) ) {
				auto &&subtrahend = cxxmath::get_node<mathutils::number>( tch.back() );
				return ( minuend.data - subtrahend.data );
			}
			
			if( minuend.data == mathutils::number{ 0, 0 } ) {
				return this->operator()(
					mathutils::negation{},
					std::array<model::lagrangian_tree, 1>{ std::move( ch.back() ) },
					std::array<model::lagrangian_tree, 1>{ std::move( tch.back() ) }
				);
			}
		} else if( cxxmath::holds_node<mathutils::number>( tch.back() ) ) {
			auto &&subtrahend = cxxmath::get_node<mathutils::number>( tch.back() );
			
			if( subtrahend.data == mathutils::number{ 0, 0 } )
				return tch.front();
		}
		
		return { mathutils::difference{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::quotient, Children &&, TransformedChildren &&tch ) const {
		BOOST_LOG_NAMED_SCOPE( "simplify_arithmetic_impl::operator()()" );
		
		if( cxxmath::holds_node<mathutils::number>( tch.front() ) ) {
			auto &&dividend = cxxmath::get_node<mathutils::number>( tch.front() );
			
			if( cxxmath::holds_node<mathutils::number>( tch.back() ) ) {
				auto &&divisor = cxxmath::get_node<mathutils::number>( tch.back() );
				return ( dividend.data / divisor.data );
			}
			
			if( dividend.data == mathutils::number{ 0, 0 } )
				return mathutils::number{ 0, 0 };
		} else if( cxxmath::holds_node<mathutils::number>( tch.back() ) ) {
			auto &&divisor = cxxmath::get_node<mathutils::number>( tch.back() );
			
			if( divisor.data == mathutils::number{ 0, 0 } ) {
				BOOST_LOG_SEV( logger, logging::severity_level::error )
					<< "Division by zero encountered";
				error::exit_upon_error();
			}
			
			if( divisor.data == mathutils::number{ 1, 0 } )
				return tch.front();
		}
		
		return { mathutils::quotient{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::power, Children &&, TransformedChildren &&tch ) const {
		if( cxxmath::holds_node<mathutils::number>( tch.front() ) ) {
			auto &&basis = cxxmath::get_node<mathutils::number>( tch.front() );
			
			if( cxxmath::holds_node<mathutils::number>( tch.back() ) ) {
				auto &&exponent = cxxmath::get_node<mathutils::number>( tch.back() );
				// FIXME: Call a power taking function
				return { mathutils::power{}, std::move( basis ), std::move( exponent ) };
			}
			
			if( basis.data == mathutils::number{ 0, 0 } )
				return mathutils::number{ 0, 0 };
			if( basis.data == mathutils::number{ 1, 0 } )
				return mathutils::number{ 1, 0 };
		} else if( cxxmath::holds_node<mathutils::number>( tch.back() ) ) {
			auto &&exponent = cxxmath::get_node<mathutils::number>( tch.back() );
			
			if( exponent.data == mathutils::number{ 0, 0 } )
				return mathutils::number{ 1, 0 };
			if( exponent.data == mathutils::number{ 1, 0 } )
				return tch.front();
		}
		
		return { mathutils::power{}, std::forward<TransformedChildren>( tch ) };
	}
	
	template<class Children, class TransformedChildren>
	model::lagrangian_tree operator()( mathutils::negation, Children &&, TransformedChildren &&tch ) const {
		if( cxxmath::holds_node<mathutils::number>( *std::begin( tch ) ) ) {
			return - cxxmath::get_node<mathutils::number>( *std::begin( tch ) ).data;
		}
		if( cxxmath::holds_node<mathutils::negation>( *std::begin( tch ) ) ) {
			return cxxmath::get_node<mathutils::negation>( *std::begin( tch ) ).children.front();
		}
		
		return { mathutils::negation{}, std::forward<TransformedChildren>( tch ) };
	}
};
}

namespace PQuantum::calculation {
model::lagrangian_tree simplify_arithmetic( const model::lagrangian_tree &lagrangian ) {
	return cxxmath::recursive_tree_transform( lagrangian, simplify_arithmetic_impl{} );
}
}