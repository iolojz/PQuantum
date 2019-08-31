//
// Created by jayz on 31.08.19.
//

#ifndef PQUANTUM_ABSTRACT_INDEX_NOTATION_HPP
#define PQUANTUM_ABSTRACT_INDEX_NOTATION_HPP

#include "mathutils/abstract_algebra/free_polynomial.hpp"

namespace PQuantum
{
namespace mathutils
{
template<class Variable, class Coefficient, class IndexContractor, class LessVariables = std::less<Variable>, class CoefficientRingTag = void>
class abstract_index_quotient
{
// FIXME: Due to the gcc bug #91609, these things have to be public...
public:
	using variable = Variable;
	using coefficient = Coefficient;
	using less_variables = LessVariables;
	using coefficient_ring_tag = CoefficientRingTag;
	using coefficient_ring = abstract_algebra::ring<coefficient, coefficient_ring_tag>;
	
	template<class V, class C, class Less, class CRTag> using quotient_alias = clifford_quotient<V, C, IndexContractor, Less, CRTag>;
	using abstract_index_polynomial = free_polynomial_quotient<variable, coefficient, quotient_alias, less_variables>;
	friend abstract_index_polynomial;
	using underlying_polynomial = typename abstract_index_polynomial::underlying_polynomial;
	
	static std::pair<underlying_polynomial, bool>
	operator()( const coefficient &c, const std::vector<variable> &variables )
	{
		const auto index_getter = IndexContractor::index_getter{};
		const auto less_indices = IndexGetter::less_indices{};
		IndexContractor index_contractor;
		
		underlying_polynomial contracted;
		auto it = std::adjacent_find( variables.begin(), variables.end(),
									  [&contracted]( const auto &v1, const auto &v2 ) {
										  auto v1_indices = std::sort( index_getter( v1 ), less_indices );
										  auto v2_indices = std::sort( index_getter( v2 ), less_indices );
			
										  decltype( v1_indices ) common_indices;
										  std::set_intersection( v1_indices.begin(), v1_indices.end(),
																 v2_indices.begin(), v2_indices.end(),
																 std::back_inserter( intersection ));
			
										  if( common_indices.empty() == false ) {
											  contracted = index_contractor( v1, v2, std::move( common_indices ));
											  return true;
										  }
			
										  return false;
									  } );
		
		if( it != variables.end()) {
			contracted = ( underlying_polynomial{ c, { variables.begin(), it }} * std::move( contracted ) *
						   underlying_polynomial{ coefficient_ring::one(), { it + 2, variables.end() }} );
			return std::make_pair( canonicalize( std::move( contracted )), true );
		}
		
		auto c_indices = std::sort( index_getter( c ), less_indices );
		for( auto v_it = variables.begin(); v_it != variables.end(); ++v_it ) {
			auto v_indices = std::sort( index_getter( *v_it ), less_indices );
			decltype( v_indices ) common_indices;
			std::set_intersection( c_indices.begin(), c_indices.end(), v_indices.begin(), v_indices.end(),
								   std::back_inserter( intersection ));
			
			if( common_indices.empty() == false ) {
				contracted = index_contractor( c, *v_it, std::move( common_indices ));
				contracted = ( underlying_polynomial{ coefficient_ring::one(), { variables.begin(), v_it }} *
							   std::move( contracted ) *
							   underlying_polynomial{ coefficient_ring::one(), { ++v_it, variables.end() }} );
				return std::make_pair( canonicalize( std::move( contracted )), true );
			}
		}
	}
	
	static void canonicalize( abstract_index_polynomial &p )
	{
		std::vector<std::pair<typename decltype(p.monomial_map)::iterator, underlying_polynomial>> canonicalized;
		
		for( auto it = p.monomial_map.begin(); it != p.monomial_map.end(); ++it ) {
			auto contraction_pair = IndexContractor{}( it->second, it->first );
			if( contraction_pair.second == false )
				canonicalized.emplace_back( it, std::move( contraction_pair.first ));
		}
		
		for( const auto &c : canonicalized )
			p.monomial_map.erase( c.first );
		for( auto &c : canonicalized )
			static_cast<underlying_polynomial &>( p ) += std::move( c.second );
	}
	
	static underlying_polynomial expand_all_summations( const abstract_index_polynomial &p )
	{
	
	}
	
	static bool equal( const abstract_index_polynomial &p1, const abstract_index_polynomial &p2 )
	{ return ( expand_all_summations( p1 ) == expand_all_summations( p2 ); }

public:
	using type = abstract_index_polynomial;
};

template<class BilinearForm, class IndexGetter, class UnderlyingPolynomial = void>
class gamma_index_contractor
{
	using index_getter = IndexGetter;
	using less_indices = LessIndices;
	
	// Note that Variable1/2 will in general __not__ be gamma_matrix!
	template<class Variable1, class Variable2>
	auto operator()( Variable1 &&, Variable2 &&, const std::vector<spacetime_index> &indices ) const
	{
		BOOST_LOG_NAMED_SCOPE( "gamma_index_contractor::operator()()" );
		io::severity_logger logger;
		
		if( indices.size() != 1 ) {
			BOOST_LOG_SEV( logger, io::severity_level::internal_error ) << "Can only contract one index. Attempted: "
																		<< indices.size();
			error::exit_upon_error();
		}
		
		static_assert( std::is_same_v<std::decay_t<Variable1>, std::decay_t<Variable2>>,
					   "Different types of variables encountered." );
		using variable = std::decay_t<Variable1>;
		
		using underlying_polynomial = std::conditional_t<std::is_void_v<UnderlyingPolynomial>, UnderlyingPolynomial, free_polynomial<variable, decltype( coefficient )> >;
		
		auto coefficient = BilinearForm{}( gamma_matrix{ indices.front() }, gamma_matrix{ indices.front() } );
		return underlying_polynomial{ std::move( c ) };
	}
	
	static underlying_polynomial expand_all_summations( const coefficient &c, const std::vector<variable> &v );
};
}

#endif //PQUANTUM_ABSTRACT_INDEX_NOTATION_HPP
