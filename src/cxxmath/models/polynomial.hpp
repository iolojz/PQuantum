//
// Created by jayz on 03.10.19.
//

#ifndef CXXMATH_MODELS_POLYNOMIAL_HPP
#define CXXMATH_MODELS_POLYNOMIAL_HPP

#include "std_vector.hpp"

namespace cxxmath
{
template<class Coefficient, class Variable, class CoefficientSet = default_set_t <tag_of_t<Coefficient>>, class CoefficientRing = default_ring_t <tag_of_t<Coefficient>>, class TotalVariableOrder = default_total_order_t <tag_of_t<Variable>>>
struct polynomial_tag
{
};

template<class Coefficient, class Variable, class CoefficientSet = default_set_t <tag_of_t<Coefficient>>, class CoefficientRing = default_ring_t <tag_of_t<Coefficient>>, class TotalVariableOrder = default_total_order_t <tag_of_t<Variable>>>
struct polynomial
{
	using dispatch_tag = polynomial_tag<Coefficient, Variable, CoefficientSet, CoefficientRing, TotalVariableOrder>;
	
	using coefficient = Coefficient;
	using coefficient_set = CoefficientSet;
	using coefficient_ring = CoefficientRing;
	using variable = Variable;
	using variable_monoid = typename vector_monoid<variable>::type;
private:
	using monomial_product = std_get_product;
	using monomial_monoid = product_monoid<monomial_product, typename coefficient_ring::monoid_, variable_monoid>
	
	static constexpr auto
	add_assign_coefficients = coefficient_ring::add_assign;
	static constexpr auto equal_coefficients = coefficient_set::equal;
	static constexpr auto zero_coefficient = coefficient_ring::zero;
	static constexpr auto negate_coefficient_in_place = coefficient_ring::negate_in_place;
	
	struct less_variables
	{
		template<class V1, class V2>
		decltype( auto ) operator()( V1 &&v1, V2 &&v2 ) const
		{
			return TotalVariableOrder::less( std::forward<V1>( v1 ), std::forward<V2>( v2 ));
		}
	};
	
	struct less_variable_ranges
	{
		template<class Range1, class Range2>
		bool operator()( Range1 &&r1, Range2 &&r2 ) const
		{
			return std::lexicographical_compare( std::begin( r1 ), std::end( r1 ), std::begin( r2 ), std::end( r2 ),
												 less_variables{} );
		}
	};
	
	void strip_zeros( void )
	{
		erase_if( std::begin( monomial_map ), std::end( monomial_map ), []( const auto &monomial ) {
			return equal_coefficients( monomial_product::second( monomial ), zero_coefficient());
		} );
	}
	
	// FIXME: Should we enable runtime switching of this type?
	using monomial_container = boost::container::flat_map<std::vector<variable>, coefficient, less_variable_ranges>;
	//using monomial_container = std::map<decltype(std::declval<monomial>().variables), coefficient, less_variable_ranges>;
	monomial_container monomial_map;
public:
	free_polynomial( void ) = default;
	
	free_polynomial( const free_polynomial & ) = default;
	
	free_polynomial(free_polynomial
	&&) = default;
	
	const monomial_container &monomials( void ) const
	{ return monomial_map; }
	
	free_polynomial &operator+=( const free_polynomial &p )
	{
		if( this == &p ) {
			for( auto &term : monomial_map )
				add_assign_coefficients( term.second, term.second );
			
			return *this;
		}
		
		for( const auto &term : p.monomials()) {
			auto insertion_result = monomial_map.insert( term );
			if( insertion_result.second == false ) {
				add_assign_coefficients( insertion_result.first->second, term.second );
				if( equal_coefficients( insertion_result.first->second, zero_coefficient()))
					monomial_map.erase( insertion_result.first );
			}
		}
		
		return *this;
	}
	
	free_polynomial &operator+=( free_polynomial &&p )
	{
		if( this == &p ) {
			for( auto &term : monomial_map )
				add_assign_coefficients( term.second, term.second );
			return *this;
		}
		
		for( auto &&term : p.monomials()) {
			auto insertion_result = monomial_map.insert( term );
			if( insertion_result.second == false ) {
				add_assign_coefficients( insertion_result.first->second, std::move( term.second ));
				if( equal_coefficients( insertion_result.first->second, zero_coefficient()))
					monomial_map.erase( insertion_result.first );
			}
		}
		
		return *this;
	}
	
	template<class Polynomial>
	free_polynomial &operator-=( Polynomial &&p )
	{
		return *this += -std::forward<Polynomial>( p );
	}
	
	CXXMATH_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial,
	+)
	
	CXXMATH_BINARY_OPERATOR_OVERLOAD( free_polynomial,
	- )
	
	free_polynomial &negate_in_place( void )
	{
		for( auto &term : monomial_map )
			negate_coefficient_in_place( term.second );
		
		return std::move( *this );
	}
	
	free_polynomial operator-( void ) &&
	{
		negate_in_place();
		return std::move( *this );
	}
	
	free_polynomial operator-( void ) const &
	{ return -free_polynomial{ *this }; }
	
	free_polynomial &operator*=( const free_polynomial &p )
	{
		std::vector<monomial_container::value_type> monomials;
		for( const auto &factor1 : monomial_map ) {
			for( const auto &factor2 : p.monomial_map ) {
				monomials.push_back( monomial_monoid::multiply( factor1, factor2 ));
			}
		}
		
		monomial_container result;
		for( auto &&monomial : monomials ) {
			if( equal_coefficients( monomial.second, zero_coefficient()))
				continue;
			
			auto insertion_result = result.insert( monomial );
			if( insertion_result.second == false ) {
				add_assign_coefficients( insertion_result.first->second, std::move( monomial.coefficient ));
				if( equal_coefficients( insertion_result.first->second, zero_coefficient()))
					result.erase( insertion_result.first );
			}
		}
		
		monomial_map = std::move( result );
		return *this;
	}
	
	CXXMATH_BINARY_OPERATOR_OVERLOAD(free_polynomial, *
	)
	
	bool operator==( const free_polynomial &p ) const
	{
		if( monomial_map.size() != p.monomial_map.size())
			return false;
		
		for( const auto &monomials : boost::combine( monomial_map, p.monomial_map )) {
			const auto &variables1 = boost::get<0>( monomials ).first;
			const auto &variables2 = boost::get<1>( monomials ).first;
			
			if( variables1.size() != variables2.size())
				return false;
			
			for( const auto &variables : boost::combine( variables1, variables2 )) {
				if( TotalVariableOrder::not_equal( boost::get<0>( variables ), boost::get<1>( variables )))
					return false;
			}
			
			if( !equal_coefficients( boost::get<0>( monomials ).second, boost::get<1>( monomials ).second ))
				return false;
		}
		
		return true;
	}
	
	bool operator!=( const free_polynomial &p ) const
	{ return !( *this == p ); }
};

namespace impl
{
template<class Tag>
struct is_polynomial_tag : std::false_type
{
};

template<class Coefficient, class Variable, class CoefficientSet, class CoefficientRing, class TotalVariableOrder>
struct is_polynomial_tag<Coefficient, Variable, CoefficientSet, CoefficientRing, TotalVariableOrder> : std::true_type
{
};

struct supports_polynomial_tag
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return is_polynomial_tag<Tag>::value;
	}
};

struct polynomial_add_assign : supports_polynomial_tag
{
	template<class Polynomial1, class Polynomial2>
	static constexpr decltype( auto ) apply( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return std::forward<Polynomial1>( p1 ) += std::forward<Polynomial2>( p2 );
	}
};

struct polynomial_addition_is_abelian
{
	static constexpr bool apply( void )
	{
		return true;
	}
};

template<class Coefficient, class Variable, class CoefficientSet, class CoefficientRing, class TotalVariableOrder>
struct polynomial_zero
{
	static constexpr auto apply( void )
	{
		return polynomial<Coefficient, Variable, CoefficientSet, CoefficientRing, TotalVariableOrder>{};
	}
};

struct polynomial_negate : supports_polynomial_tag
{
	template<class Polynomial>
	static constexpr bool apply( Polynomial &&p )
	{
		return std::forward<Polynomial>( p ).negate_in_place();
	}
};

struct polynomial_multiply_assign : supports_polynomial_tag
{
	template<class Polynomial1, class Polynomial2>
	static constexpr decltype( auto ) apply( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return std::forward<Polynomial1>( p1 ) *= std::forward<Polynomial2>( p2 );
	}
};

struct polynomial_multiplication_is_abelian
{
	static constexpr bool apply( void )
	{
		return false;
	}
};

template<class Coefficient, class Variable, class CoefficientSet, class CoefficientRing, class TotalVariableOrder>
struct polynomial_one
{
	static constexpr auto apply( void )
	{
		return polynomial<Coefficient, Variable, CoefficientSet, CoefficientRing, TotalVariableOrder>{
		std::make_pair( CoefficientRing::one(), std::vector<Variable>{} ); };
	}
};
}

template<class Coefficient, class Variable, class CoefficientSet = default_set_t <tag_of_t<Coefficient>>, class CoefficientRing = default_ring_t <tag_of_t<Coefficient>>, class TotalVariableOrder = default_total_order_t <tag_of_t<Variable>>> using polynomial_ring = concepts::ring<concepts::assignable_group<concepts::assignable_monoid<impl::polynomial_add_assign, polynomial_zero, polynomial_addition_is_abelian>, impl::polynomial_negate_in_place>, concepts::assignable_monoid<impl::polynomial_multiply_assign, polynomial_one, polynomial_multiplication_is_abelian> >;
}

#endif //CXXMATH_MODELS_POLYNOMIAL_HPP
