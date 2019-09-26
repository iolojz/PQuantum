//
// Created by jayz on 24.09.19.
//

#ifndef TAGD_MODELS_CLIFFORD_HPP
#define TAGD_MODELS_CLIFFORD_HPP

#include "free_polynomial.hpp"
#include "quotient.hpp"

namespace tag_dispatch
{
namespace models
{
template<class BilinearForm, class TotalVariableOrder, class StructureTag>
struct clifford_quotient
{
};

template<class Coefficient, class Variable, class BilinearForm, class TotalVariableOrder, class StructureTag>
struct coefficient_ring_for_polynomial<Coefficient, Variable, clifford_quotient<BilinearForm, TotalVariableOrder, StructureTag>>
{
	using type = typename coefficient_ring_for_polynomial<Coefficient, Variable, StructureTag>::type;
};

template<class Coefficient, class Variable, class BilinearForm, class TotalVariableOrder>
struct variable_total_order_for_polynomial<Coefficient, Variable, clifford_quotient<BilinearForm, TotalVariableOrder, StructureTag>>
{
	using type = TotalVariableOrder;
};

template<class PolynomialTag, class CliffordQuotient>
struct underlying_polynomial_of_clifford;
template<class PolynomialTag, class BilinearForm, class TotalVariableOrder, class StructureTag>
struct underlying_polynomial_of_clifford<PolynomialTag, clifford_quotient<BilinearForm, TotalVariableOrder, StructureTag>>
{
	using type = polynomial_for_tag<PolynomialTag, concepts::ring<PolynomialTag, StructureTag>>;
};

template<class Variable, class BilinearForm>
struct value_type_of_biinear_form
{
	using type = std::decay_t<decltype( std::declval<BilinearForm>()( std::declval<Variable>(),
																	  std::declval<Variable>()))>;
};
template<class Variable, class BilinearForm> using value_type_of_biinear_form_t = typename value_type_of_biinear_form<Variable, BilinearForm>::type;

template<class Variable, class BilinearForm, class TotalVariableOrder = concepts::total_order<tag_of_t<Variable>, void>, class CoefficientRing = concepts::ring<value_type_of_biinear_form_t<Variable, BilinearForm>, void>, class StructureTag = void> using clifford = quotient_wrapper<typename underlying_polynomial_of_clifford<
polynomial_tag <
value_type_of_biinear_form<Variable, BilinearForm>, Variable>, clifford_quotient<BilinearForm, TotalVariableOrder, StructureTag> >::type,
clifford_quotient<BilinearForm, TotalVariableOrder, StructureTag>
>;
}

namespace impl
{
namespace detail
{
template<class Clifford>
struct clifford_canonicalizer;

template<class Variable, class BilinearForm, class TotalVariableOrder, class CoefficientRing>
class clifford_canonicalizer<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing>>
{
	using polynomial = models::underlying_polynomial_of_clifford_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing>>;
	using monomial = typename polynomial::monomial;
	using coefficient = tag_of_t<typename polynomial::coefficient>;
	
	using equal_variables = impl::not_equal<tag_of_t<Variable>, TotalVariableOrder>;
	
	struct less_variables
	{
		template<class V1, class V2>
		decltype( auto ) operator()( V1 &&v1, V2 &&v2 ) const
		{
			using less = impl::less<tag_of_t<Variable>, TotalVariableOrder>;
			return less::apply( std::forward<V1>( v1 ), std::forward<V2>( v2 ));
		}
	};
	
	static polynomial canonicalize_monomial( monomial &&m )
	{
		auto variables = std::move( m.variables );
		auto last_ordered = std::adjacent_find( variables.begin(), variables.end(), std::not_fn( less_variables{} ));
		
		if( last_ordered == variables.end())
			return { std::move( m.coeff ), std::make_move_iterator( variables.begin()),
					 std::make_move_iterator( variables.end()) };
		
		auto clifford_coefficient = BilinearForm{}( *last_ordered, *( last_ordered + 1 ));
		if( equal_variables::apply( *last_ordered, *( last_ordered + 1 ))) {
			auto shortened_coefficient = std::move( m.coeff ) * std::move( clifford_coefficient );
			variables.erase( last_ordered, last_ordered + 2 );
			
			return canonicalize_monomial( { std::move( variables ), std::move( shortened_coefficient ) } );
		}
		
		auto permuted_coefficient = -m.coeff;
		std::swap( *last_ordered, *( last_ordered + 1 ));
		auto canonicalized_polynomial = canonicalize_monomial(
		monomial{ variables, std::move( permuted_coefficient ) } );
		
		auto one_ = one<tag_of_t<coefficient>, CoefficientRing>::apply();
		auto two = one_ + one_;
		auto shortened_coefficient = two * std::move( m.coeff ) * std::move( clifford_coefficient );
		variables.erase( last_ordered, last_ordered + 2 );
		canonicalized_polynomial += canonicalize_monomial(
		monomial{ std::move( variables ), std::move( shortened_coefficient ) } );
		
		return canonicalized_polynomial;
	}

public:
	polynomial operator()( polynomial p ) const
	{
		polynomial canonicalized;
		for( const auto &map_entry : p.monomials())
			canonicalized += canonicalize_monomial(
			{{ std::make_move_iterator( map_entry.first.begin()), std::make_move_iterator( map_entry.first.end()) },
			 std::move( map_entry.second ) } );
		return canonicalized;
	}
};
}

template<class Variable, class BilinearForm, class TotalVariableOrder, class CoefficientRing>
struct to<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, tag_of_t<models::underlying_polynomial_of_clifford_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>,
clifford_quotient < BilinearForm, TotalVariableOrder, void> >
{
private:
	using clifford_type = models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>;
	using clifford_quotient_ = typename clifford_type::structure_tag;
	using polynomial_type = typename clifford_type::underlying_type;
public:
	template<class Polynomial>
	static clifford_type apply( Polynomial &&p )
	{
		return wrap_quotient_representative < polynomial_type, clifford_quotient_
		>> ( detail::clifford_canonicalizer<clifford_type>{}( std::forward<Polynomial>( p )));
	}
};

template<class Variable, class BilinearForm, class TotalVariableOrder, class CoefficientRing>
struct make<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, concepts::makeable<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>
clifford_quotient < BilinearForm, TotalVariableOrder, void> >
{
private:
	using clifford_type = models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>;
	using polynomial = typename clifford_type::underlying_type;
public:
	template<class ...Args>
	static constexpr auto apply( Arg &&...args )
	{
		auto p = make<tag_of_t<polynomial>, concepts::makeable<tag_of_t<polynomial>, void>>::apply(
		std::forward<Args>( args )... );
		return detail::clifford_canonicalizer<clifford_type>{}( std::move( p ));
	}
};

template<class Variable, class BilinearForm, class TotalVariableOrder, class CoefficientRing>
struct is_abelian<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, concepts::monoid<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>> void> >
{
	static constexpr bool apply( void )
	{ return true; }
};

template<class Variable, class BilinearForm, class TotalVariableOrder, class CoefficientRing>
struct neutral_element<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, concepts::monoid<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, void> >
{
private:
	using clifford_type = models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>;
	using polynomial = typename clifford_type::underlying_type;
public:
	static constexpr auto apply( void )
	{
		return detail::clifford_canonicalizer<clifford_type>{}( polynomial{} );
	}
};

template<class Variable, class BilinearForm, class TotalVariableOrder, class CoefficientRing>
struct one<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, concepts::monoid<tag_of_t<models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>>, void> >
{
private:
	using clifford_type = models::clifford<Variable, BilinearForm, TotalVariableOrder, CoefficientRing, void>;
	using polynomial = typename clifford_type::underlying_type;
	using coefficient = typename polynomial::coefficient;
	using coefficient_ring = typename polynomial::coefficient_ring;
public:
	static constexpr bool apply( void )
	{
		auto p = polynomial{ monomial{ one<tag_of_t<coefficient>, coefficient_ring>::apply() }};
		return detail::clifford_canonicalizer<clifford_type>{}( std::move( p ));
	}
};
}
}

#endif //TAGD_MODELS_CLIFFORD_HPP
