//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_FREE_POLYNOMIAL_HPP
#define PQUANTUM_FREE_POLYNOMIAL_HPP

#include <map>
#include <vector>
#include <boost/range/combine.hpp>

#include "template_helpers/erase_if.hpp"
#include "template_helpers/comparisons.hpp"
#include "operator_helpers.hpp"
#include "ring.hpp"

namespace PQuantum {
namespace mathutils {
template<class Variable, class Coefficient, class LessVariables = std::less<Variable>, class CoefficientRingTag = void>
class free_polynomial {
public:
	using coefficient = Coefficient;
	using coefficient_ring = abstract_algebra::ring<coefficient, CoefficientRingTag>;
	using variable = Variable;
	using less_variables = LessVariables;
protected:
	struct less_variable_vectors
	{
		template<class Vector1, class Vector2>
		bool operator()( Vector1 &&v1, Vector2 &&v2 ) const
		{
			return std::lexicographical_compare( v1.begin(), v1.end(), v2.begin(), v2.end(), less_variables{} );
		}
	};
	
	std::map<std::vector<Variable>, Coefficient, less_variable_vectors> monomial_map;
	
	free_polynomial( decltype( monomial_map ) &&m ) : monomial_map{ std::move( m ) }
	{}
private:
	void strip_zeros(void) {
		template_helpers::erase_if(monomial_map, [](auto &&map_entry) {
			return coefficient_ring::equal(map_entry.second, coefficient_ring::zero());
		});
	}
public:
	struct monomial
	{
		std::vector<variable> variables;
		coefficient coeff;
	};
	
	free_polynomial(void) = default;
	
	free_polynomial( const monomial &m ) : monomial_map{{ m.variables, m.coefficient }}
	{ strip_zeros(); }
	
	free_polynomial( monomial &&m ) : monomial_map{{ std::move( m.variables ), std::move( m.coeff ) }}
	{ strip_zeros(); }
	
	template<class InputIterator>
	free_polynomial(const coefficient &c, InputIterator begin, InputIterator end)
			: monomial_map{{{begin, end}, c}} { strip_zeros(); }
	
	template<class InputIterator>
	free_polynomial(coefficient &&c, InputIterator begin, InputIterator end)
			: monomial_map{{{begin, end}, std::move(c)}} { strip_zeros(); }
	
	explicit free_polynomial(const coefficient &c) : monomial_map{{{}, c}} { strip_zeros(); }
	
	explicit free_polynomial(coefficient &&c) : monomial_map{{{}, std::move(c)}} { strip_zeros(); }
	
	free_polynomial &operator+=(const free_polynomial &p) &{
		if(&p == this) {
			for(auto &term : monomial_map)
				coefficient_ring::add_assign(term.second, term.second);
			
			return *this;
		}
		
		for(const auto &term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::add_assign(insertion_result.first->second, term.second);
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			}
		}
		
		return *this;
	}
	
	free_polynomial &operator+=(free_polynomial &&p) {
		if(&p == this) {
			for(auto &term : monomial_map)
				coefficient_ring::add_assign(term.second, term.second);
			
			return *this;
		}
		
		for(auto &&term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::add_assign(insertion_result.first->second, std::move(term.second));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			}
		}
		
		return *this;
	}
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial, +)
	
	free_polynomial &operator-=(const free_polynomial &p) {
		if(&p == this) {
			monomial_map.clear();
			return *this;
		}
		
		for(const auto &term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::subtract_assign(insertion_result.first->second, term.second);
				if(coefficient_ring::equal(insertion_result.first->second == coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			} else
				insertion_result.first->second = -insertion_result.first->second;
		}
		
		return *this;
	}
	
	free_polynomial &operator-=(free_polynomial &&p) {
		if(&p == this) {
			monomial_map.clear();
			return *this;
		}
		
		for(auto &&term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::subtract_assign(insertion_result.first->second, std::move(term.second));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			} else
				insertion_result.first->second = -insertion_result.first->second;
		}
		
		return *this;
	}
	
	PQUANTUM_BINARY_OPERATOR_OVERLOAD(free_polynomial, -, -)
	
	free_polynomial operator-(void) &&{
		for(auto &term : monomial_map)
			coefficient_ring::negate_in_place(term.second);
		
		return std::move(*this);
	}
	
	free_polynomial operator-(void) const &{ return -free_polynomial{*this}; }
	
	free_polynomial &operator*=(const free_polynomial &p) {
		std::vector<monomial> monomials;
		for(const auto &factor1 : monomial_map) {
			for(const auto &factor2 : p.monomial_map) {
				auto concatenated_variables = factor1.first;
				concatenated_variables.insert(concatenated_variables.end(), factor2.first.begin(), factor2.first.end());
				
				monomials.push_back({std::move(concatenated_variables), factor1.second * factor2.second});
			}
		}
		
		decltype(monomial_map) result;
		for(auto &&monomial : monomials) {
			if( coefficient_ring::equal( monomial.coeff, coefficient_ring::zero()))
				continue;
			
			auto insertion_result = result.insert( { monomial.variables, monomial.coeff } );
			if(insertion_result.second == false) {
				coefficient_ring::add_assign( insertion_result.first->second, std::move( monomial.coeff ));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					result.erase(insertion_result.first);
			}
		}
		
		monomial_map = std::move(result);
		return *this;
	}
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial, *)
	
	bool operator==(const free_polynomial &p) const {
		if(monomial_map.size() != p.monomial_map.size())
			return false;
		
		for( const auto &map_entries : boost::combine( monomial_map, p.monomial_map )) {
			const auto &variables1 = boost::get<0>( map_entries ).first;
			const auto &variables2 = boost::get<1>( map_entries ).first;
			
			if(variables1.size() != variables2.size())
				return false;
			
			for( const auto &variables : boost::combine( variables1, variables2 )) {
				if( !template_helpers::equal( boost::get<0>( variables ), boost::get<1>( variables ),
											  less_variables{} ))
					return false;
			}
			
			if( !coefficient_ring::equal( boost::get<0>( map_entries ).second, boost::get<0>( map_entries ).second ))
				return false;
		}
		
		return true;
	}
	
	auto monomials(void) const { return monomial_map; }
};

namespace abstract_algebra {
template<class Variable, class Coefficient, class LessVariables, class CoefficientRingTag>
struct set_impl<free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>>
{
	template<class Polynomial1, class Polynomial2>
	static bool equal( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return ( std::forward<Polynomial1>( p1 ) == std::forward<Polynomial2>( p2 ));
	}
};

template<class Variable, class Coefficient, class LessVariables, class CoefficientRingTag>
struct group_impl<free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>>
{
private:
	using polynomial = free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>;
public:
	static constexpr bool is_abelian = true;
	
	template<class Polynomial1, class Polynomial2>
	static constexpr decltype( auto ) compose_assign( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return ( std::forward<Polynomial1>( p1 ) += std::forward<Polynomial2>( p2 ));
	}
	
	template<class Polynomial>
	static constexpr decltype(auto) inverse(Polynomial &&p) { return -std::forward<Polynomial>(p); }
	
	static constexpr decltype(auto) neutral_element() { return polynomial{}; }
};

template<class Variable, class Coefficient, class LessVariables, class CoefficientRingTag>
struct ring_impl<free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>>
{
private:
	using polynomial = free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>;
public:
	template<class Polynomial1, class Polynomial2>
	static decltype( auto ) multiply_assign( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return ( std::forward<Polynomial1>( p1 ) *= std::forward<Polynomial2>( p2 ));
	}
	
	static auto one()
	{ return polynomial{ ring<Coefficient, CoefficientRingTag>::one() }; }
};
}

#ifdef PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD
#error "PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD already defined"
#endif

#define PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(op) \
free_polynomial_quotient &operator op ## =( const free_polynomial_quotient &c ) &\
{ \
    *static_cast<base *>( this ) op ## = c; \
    quotient::canonicalize( *this ); \
    return *this; \
} \
\
free_polynomial_quotient &&operator op ## =( const free_polynomial_quotient &c ) && \
{ \
    std::move( *static_cast<base *>( this ) ) op ## = c; \
    quotient::canonicalize( *this ); \
    return std::move( *this ); \
} \
free_polynomial_quotient &operator op ## =( free_polynomial_quotient &&c ) & \
{ \
    *static_cast<base *>( this ) op ## = std::move( c ); \
    quotient::canonicalize( *this ); \
    return *this; \
} \
\
free_polynomial_quotient &&operator op ## =( free_polynomial_quotient &&c ) && \
{ \
    std::move( *static_cast<base *>( this ) ) op ## = std::move( c ); \
    quotient::canonicalize( *this ); \
    return std::move( *this ); \
}

template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables = std::less<Variable>, class CoefficientRingTag = void>
class free_polynomial_quotient
		: protected free_polynomial<typename Quotient<Variable, Coefficient, LessVariables, CoefficientRingTag>::variable, typename Quotient<Variable, Coefficient, LessVariables, CoefficientRingTag>::coefficient, typename Quotient<Variable, Coefficient, LessVariables, CoefficientRingTag>::less_variables, typename Quotient<Variable, Coefficient, LessVariables, CoefficientRingTag>::coefficient_ring_tag> {
	using quotient = Quotient<Variable, Coefficient, LessVariables, CoefficientRingTag>;
	using base = free_polynomial<typename quotient::variable, typename quotient::coefficient, typename quotient::less_variables, typename quotient::coefficient_ring_tag>;
	friend quotient;
public:
	using variable = typename base::variable;
	using less_variables = typename base::less_variables;
	using coefficient = typename base::coefficient;
	using coefficient_ring_tag = typename base::coefficient_ring::tag;
	using underlying_polynomial = base;
	
	free_polynomial_quotient(void) = default;
	
	free_polynomial_quotient( const free_polynomial_quotient & ) = default;
	
	free_polynomial_quotient( free_polynomial_quotient && ) = default;
	
	template<class C, class InputIterator>
	free_polynomial_quotient(C &&c, InputIterator begin, InputIterator end)
			: base{std::forward<C>(c), begin, end} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(const base &p) : base{p} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(base &&p) : base{std::move(p)} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(const coefficient &c) : base{c} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(coefficient &&c) : base{std::move(c)} { quotient::canonicalize(*this); }
	
	PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(+)
	
	PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(-)
	
	PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(*)
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial_quotient, +)
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial_quotient, *)
	
	PQUANTUM_BINARY_OPERATOR_OVERLOAD(free_polynomial_quotient, -, -)
	
	using base::operator==;
	using base::monomials;
};

#undef PQUANTUM_QUOTIENT_BOUND_OPERATOR_OVERLOAD

namespace abstract_algebra
{
template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag, class Tag>
struct set_impl<free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>, Tag>
{
private:
	using polynomial = typename free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>::underlying_polynomial;
	using polynomial_set = set<polynomial, Tag>;
public:
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( equal, polynomial_set, equal )
};

template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag, class Tag>
struct group_impl<free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>, Tag>
{
private:
	using polynomial = typename free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>::underlying_polynomial;
	using polynomial_group = group<polynomial, Tag>;
public:
	static constexpr bool is_abelian = group<polynomial, Tag>::is_abelian;
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( compose_assign, polynomial_group, compose_assign )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( inverse, polynomial_group, inverse )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( neutral_element, polynomial_group, neutral_element )
};

template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag, class Tag>
struct ring_impl<free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>, Tag>
{
private:
	using polynomial = typename free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>::underlying_polynomial;
	using polynomial_ring = ring<polynomial, Tag>;
public:
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( multiply_assign, polynomial_ring, multiply_assign )
	
	PQUANTUM_DEFINE_FORWARDING_STATIC_FUNCTION( one, polynomial_ring, one )
};
}
}
}

#endif //PQUANTUM_FREE_POLYNOMIAL_HPP
