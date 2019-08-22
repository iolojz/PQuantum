//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_FREE_POLYNOMIAL_HPP
#define PQUANTUM_FREE_POLYNOMIAL_HPP

#include <map>
#include <vector>

namespace PQuantum
{
namespace mathutils
{
template<class Variable, class Coefficient>
class free_polynomial
{
	struct monomial
	{
		std::vector<Variable> variables;
		Coefficient coefficient;
	};
	
	std::map<std::vector<Variable>, Coefficient> monomial_map;
public:
	free_polynomial( void ) = default;
	
	template<class InputIterator>
	free_polynomial( const Coefficient &c, InputIterator begin, InputIterator end )
	: monomial_map{{{ begin, end }, c }}
	{}
	
	template<class InputIterator>
	free_polynomial( Coefficient &&c, InputIterator begin, InputIterator end )
	: monomial_map{{{ begin, end }, std::move( c ) }}
	{}
	
	explicit free_polynomial( const Coefficient &c ) : monomial_map{{{}, c }}
	{}
	
	explicit free_polynomial( Coefficient &&c ) : monomial_map{{{}, std::move( c ) }}
	{}
	
	free_polynomial &operator+=( const free_polynomial &p )
	{
		if( &p == this ) {
			for( auto &term : monomial_map )
				term.second += term.second;
			
			return *this;
		}
		
		for( const auto &term : p.monomial_map ) {
			auto insertion_result = monomial_map.insert( term );
			if( insertion_result.second == false ) {
				insertion_result.first->second += term.second;
				if( insertion_result.first->second == Coefficient{} )
					monomial_map.erase( insertion_result.first );
			}
		}
		
		return *this;
	}
	
	free_polynomial &operator+=( free_polynomial &&p )
	{
		if( &p == this ) {
			for( auto &term : monomial_map )
				term.second += term.second;
			
			return *this;
		}
		
		for( auto &&term : p.monomial_map ) {
			auto insertion_result = monomial_map.insert( term );
			if( insertion_result.second == false ) {
				insertion_result.first->second += std::move( term.second );
				if( insertion_result.first->second == Coefficient{} )
					monomial_map.erase( insertion_result.first );
			}
		}
		
		return *this;
	}
	
	free_polynomial operator+( const free_polynomial &p ) const &
	{ return free_polynomial{ *this } += p; }
	
	free_polynomial operator+( const free_polynomial &p ) &&
	{ return std::move( *this ) += p; }
	
	free_polynomial operator+( free_polynomial &&p ) const &
	{ return std::move( p ) += *this; }
	
	free_polynomial operator+( free_polynomial &&p ) &&
	{ return std::move( *this ) += std::move( p ); }
	
	free_polynomial &operator-=( const free_polynomial &p )
	{
		if( &p == this ) {
			monomial_map.clear();
			return *this;
		}
		
		for( const auto &term : p.monomial_map ) {
			auto insertion_result = monomial_map.insert( term );
			if( insertion_result.second == false ) {
				insertion_result.first->second -= term.second;
				if( insertion_result.first->second == Coefficient{} )
					monomial_map.erase( insertion_result.first );
			} else
				insertion_result.first->second = -insertion_result.first->second;
		}
		
		return *this;
	}
	
	free_polynomial &operator-=( free_polynomial &&p )
	{
		if( &p == this ) {
			monomial_map.clear();
			return *this;
		}
		
		for( auto &&term : p.monomial_map ) {
			auto insertion_result = monomial_map.insert( term );
			if( insertion_result.second == false ) {
				insertion_result.first->second -= std::move( term.second );
				if( insertion_result.first->second == Coefficient{} )
					monomial_map.erase( insertion_result.first );
			} else
				insertion_result.first->second = -insertion_result.first->second;
		}
		
		return *this;
	}
	
	free_polynomial operator-( const free_polynomial &p ) const &
	{ return free_polynomial{ *this } -= p; }
	
	free_polynomial operator-( const free_polynomial &p ) &&
	{ return std::move( *this ) -= p; }
	
	free_polynomial operator-( free_polynomial &&p ) const &
	{ return std::move( p ) -= *this; }
	
	free_polynomial operator-( free_polynomial &&p ) &&
	{ return std::move( *this ) -= std::move( p ); }
	
	free_polynomial operator-( int ) &&
	{
		for( auto &term : monomial_map )
			term.second = -term.second;
		
		return std::move( *this );
	}
	
	free_polynomial operator-( int ) const &
	{ return -free_polynomial{ *this }; }
	
	free_polynomial &operator*=( const free_polynomial &p )
	{
		std::vector<monomial> monomials;
		for( const auto &factor1 : monomial_map ) {
			for( const auto &factor2 : p.monomial_map ) {
				auto concatenated_variables = factor1.first;
				concatenated_variables.insert( concatenated_variables.end(), factor2.first.begin(),
											   factor2.first.end());
				
				monomials.push_back( { std::move( concatenated_variables ), factor1.second * factor2.second } );
			}
		}
		
		decltype( monomial_map ) result;
		for( auto &&monomial : monomials ) {
			if( monomial.coefficient == Coefficient{} )
				continue;
			
			auto insertion_result = result.insert( { monomial.variables, monomial.coefficient } );
			if( insertion_result.second == false ) {
				insertion_result.first->second += std::move( monomial.coefficient );
				if( insertion_result.first->second == Coefficient{} )
					result.erase( insertion_result.first );
			}
		}
		
		monomial_map = std::move( result );
		return *this;
	}
	
	free_polynomial operator*( const free_polynomial &p ) const &
	{ return free_polynomial{ *this } *= p; }
	
	free_polynomial operator*( const free_polynomial &p ) &&
	{ return std::move( *this ) *= p; }
	
	free_polynomial operator*( free_polynomial &&p ) const &
	{ return std::move( p ) *= *this; }
	
	free_polynomial operator*( free_polynomial &&p ) &&
	{ return std::move( *this ) *= std::move( p ); }
	
	bool operator==( const free_polynomial &p ) const
	{ return monomial_map == p.monomial_map; }
};
}
}

#endif //PQUANTUM_FREE_POLYNOMIAL_HPP
