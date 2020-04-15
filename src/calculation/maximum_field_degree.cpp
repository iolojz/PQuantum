//
// Created by jayz on 09.04.20.
//

#include "lagrangian_manipulations.hpp"

#include <numeric>

namespace {
using namespace PQuantum;

struct field_degree_counter {
	mutable logging::severity_logger logger;
	
	template<class Atom> std::size_t operator()( Atom && ) const {
		if constexpr( std::is_same_v<std::decay_t<Atom>, model::indexed_field> )
			return 1;
		else
			return 0;
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::spacetime_derivative, Children &&, TransformedChildren &&tch ) const {
		return tch.front();
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::dirac_operator, Children &&, TransformedChildren &&tch ) const {
		return tch.front();
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::field_multiplication_operator, Children &&, TransformedChildren &&tch ) const {
		return tch.front();
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::index_sum, Children &&, TransformedChildren &&tch ) const {
		return tch.front();
	}
	
	template<class Children, class TransformedChildren>
	std::size_t
	operator()( const mathutils::function_call<std::string> &fc, Children &&, TransformedChildren &&tch ) const {
		BOOST_LOG_NAMED_SCOPE( "field_degree_counter::operator()()" );
		
		if( fc.atom == "ln" ) {
			std::size_t arg_degree = tch.front();
			if( arg_degree != 0 ) {
				BOOST_LOG_SEV( logger, logging::severity_level::error )
					<< "Expression is of infinite degree in fields.";
				error::exit_upon_error();
			}
			
			return 0;
		}
		
		std::size_t max_arg_degree = *std::max_element( std::begin( tch ), std::end( tch ) );
		if( max_arg_degree == 0 )
			return 0;
		
		BOOST_LOG_SEV( logger, logging::severity_level::error )
			<< "Unknown field degree of function \"" << fc.atom << "\".";
		error::exit_upon_error();
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::sum, Children &&, TransformedChildren &&tch ) const {
		return *std::max_element( std::begin( tch ), std::end( tch ) );
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::product, Children &&, TransformedChildren &&tch ) const {
		return std::accumulate( std::begin( tch ), std::end( tch ), 0 );
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::difference, Children &&, TransformedChildren &&tch ) const {
		return *std::max_element( std::begin( tch ), std::end( tch ) );
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::quotient, Children &&, TransformedChildren &&tch ) const {
		BOOST_LOG_NAMED_SCOPE( "field_degree_counter::operator()()" );
		std::size_t divisor_degree = tch.back();
		
		if( divisor_degree != 0 ) {
			BOOST_LOG_SEV( logger, logging::severity_level::error )
				<< "Cannot determine field degree of expression with non-zero field degree in denominator.";
			error::exit_upon_error();
		}
		
		return tch.front();
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::power, Children &&ch, TransformedChildren &&tch ) const {
		BOOST_LOG_NAMED_SCOPE( "field_degree_counter::operator()()" );
		std::size_t exponent_degree = tch.back();
		
		if( exponent_degree != 0 ) {
			BOOST_LOG_SEV( logger, logging::severity_level::error )
				<< "Expression is of infinite degree in fields.";
			error::exit_upon_error();
		}
		
		std::size_t base_degree = tch.front();
		if( base_degree != 0 ) {
			if( cxxmath::holds_node<mathutils::number>( ch.back() ) ) {
				const auto &exponent_node = cxxmath::get_node<mathutils::number>( ch.back() );
				if( exponent_node.data.is_real_integer() ) {
					if( sign( exponent_node.data.real_data() ) >= 0 ) {
						std::size_t exponent = exponent_node.data.real_data().template convert_to<std::size_t>();
						std::size_t degree = base_degree * exponent;
						
						if( degree / base_degree == exponent )
							return degree;
						
						BOOST_LOG_SEV( logger, logging::severity_level::error )
							<< "Overflow occurred.";
						error::exit_upon_error();
					}
				}
			}
			
			BOOST_LOG_SEV( logger, logging::severity_level::error )
				<< "Cannot determine field degree of expression with non-zero field degree in base "
				<< "and non-integer exponent.";
			error::exit_upon_error();
		}
		
		return 0;
	}
	
	template<class Children, class TransformedChildren>
	std::size_t operator()( mathutils::negation, Children &&, TransformedChildren &&tch ) const {
		return tch.front();
	}
};
}

namespace PQuantum::calculation {
std::size_t maximum_field_degree( const model::lagrangian_tree &expr ) {
	return cxxmath::recursive_tree_transform( expr, field_degree_counter{} );
}
}
