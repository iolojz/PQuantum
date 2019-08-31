//
// Created by jayz on 28.08.19.
//

#ifndef PQUANTUM_CLIFFORD_HPP
#define PQUANTUM_CLIFFORD_HPP

#include "mathutils/abstract_algebra/free_polynomial.hpp"

#include <algorithm>
#include <functional>

namespace PQuantum {
namespace mathutils {
template<class Variable, class Coefficient, class BilinearForm, class LessVariables = std::less<Variable>, class CoefficientRingTag = void>
class clifford_quotient {
// FIXME: Due to the gcc bug #91609, these things have to be public...
public:
	using variable = Variable;
	using coefficient = Coefficient;
	using less_variables = LessVariables;
	using coefficient_ring_tag = CoefficientRingTag;
	using coefficient_ring = abstract_algebra::ring<coefficient, coefficient_ring_tag>;
	
	template<class V, class C, class Less, class CRTag> using quotient_alias = clifford_quotient<V, C, BilinearForm, Less, CRTag>;
	using clifford = free_polynomial_quotient<variable, coefficient, quotient_alias, less_variables>;
	friend clifford;
	using underlying_polynomial = typename clifford::underlying_polynomial;
	using monomial = typename underlying_polynomial::monomial;
	
	static bool is_canonical(const std::vector<Variable> &v) {
		return (std::adjacent_find(v.begin(), v.end(), std::not_fn(LessVariables{})) == v.end());
	}
	
	static underlying_polynomial canonicalize( monomial &&m )
	{
		auto variables = std::move( m.variables );
		auto last_ordered = std::adjacent_find(variables.begin(), variables.end(), std::not_fn(LessVariables{}));
		
		if(last_ordered == variables.end())
			return { std::move( m.coeff ), std::make_move_iterator( variables.begin()),
					 std::make_move_iterator( variables.end()) };
		
		auto clifford_coefficient = BilinearForm{}(*last_ordered, *(last_ordered + 1));
		if( template_helpers::equal( *last_ordered, *( last_ordered + 1 ), less_variables{} )) {
			auto shortened_coefficient = std::move( m.coeff ) * std::move( clifford_coefficient );
			variables.erase( last_ordered, last_ordered + 2 );
			
			return canonicalize({std::move(variables), std::move(shortened_coefficient)});
		}
		
		auto permuted_coefficient = -m.coeff;
		std::swap(*last_ordered, *(last_ordered + 1));
		auto canonicalized_polynomial = canonicalize( monomial{ variables, std::move( permuted_coefficient ) } );
		
		auto two = coefficient_ring::one() + coefficient_ring::one();
		auto shortened_coefficient = two * std::move( m.coeff ) * std::move( clifford_coefficient );
		variables.erase( last_ordered, last_ordered + 2 );
		canonicalized_polynomial += canonicalize(
		monomial{ std::move( variables ), std::move( shortened_coefficient ) } );
		
		return canonicalized_polynomial;
	}
	
	static void canonicalize( clifford &p )
	{
		std::vector<std::pair<typename decltype(p.monomial_map)::iterator, underlying_polynomial>> canonicalized;
		
		for( auto it = p.monomial_map.begin(); it != p.monomial_map.end(); ++it ) {
			if(is_canonical(it->first) == false)
				canonicalized.emplace_back( it, canonicalize(
				monomial{{ std::make_move_iterator( it->first.begin()), std::make_move_iterator( it->first.end()) },
						 std::move( it->second ) } ));
		}
		
		for(const auto &c : canonicalized)
			p.monomial_map.erase(c.first);
		for(auto &c : canonicalized)
			static_cast<underlying_polynomial &>( p ) += std::move( c.second );
	}
	
	static bool equal( const clifford &c1, const clifford &c2 )
	{
		return ( static_cast<const underlying_polynomial &>( *this ) ==
				 static_cast<const underlying_polynomial &>( p ));
	}
public:
	using type = clifford;
};
}
}

#undef PQUANTUM_CLIFFORD_BOUND_OPERATOR_OVERLOAD

#endif //PQUANTUM_CLIFFORD_HPP
