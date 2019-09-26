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
namespace detail
{
template<class Variable, class BilinearForm>
struct coefficient_of_form
{
	using type = std::decay_t<decltype( std::declval<BilinearForm>()( std::declval<Variable>(),
																	  std::declval<Variable>()))>;
};
template<class Variable, class BilinearForm> using coefficient_of_form_t = typename coefficient_of_form<Variable, BilinearForm>::type;
}

template<class TotalVariableOrder, class BilinearForm>
struct clifford_tag
{
};

template<class Variable, class TotalVariableOrder, class BilinearForm, class CoefficientRing = concepts::ring<
coefficient_of_form_t < Variable, BilinearForm>, void>>
using clifford = quotient_wrapper<free_polynomial<coefficient_of_form_t <
												  Variable, BilinearForm>, Variable, CoefficientRing, TotalVariableOrder>, clifford_tag<TotalVariableOrder, BilinearForm>>;
}

namespace impl
{
template<class Variable, class TotalVariableOrder, class BilinearForm, class StructureTag>
class ToTag

,

class FromTag

,

class StructureTag

>
struct to<models::underlying_polynomial_t<models::free_polynomial_tag<Coefficient, Variable>, StructureTag>,
quotient_tag < models::free_polynomial_tag<Coefficient, Variable>, clifford_tag < TotalVariableOrder, BilinearForm>>
>
{
};

#define TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( function, concept ) \
template<class Variable, class TotalVariableOrder, class BilinearForm, class StructureTag> \
struct function< \
    models::free_polynomial_tag<Coefficient, Variable>, \
    quotient_tag<models::free_polynomial_tag<Coefficient, Variable>, clifford_tag<TotalVariableOrder, BilinearForm>>, StructureTag> \
> { \
public: \
    template<class ...Args> static constexpr decltype(auto) apply( Args &&...args ) { \
        auto polynomial = function< \
            models::free_polynomial_tag<Coefficient, Variable>, \
            concepts::makeable<models::free_polynomial_tag<Coefficient, Variable>, StructureTag> \
        >( std::forward<Args>( args )... ); \
        using clifford_t = quotient_wrapper<decltype(polynomial), clifford_tag<TotalVariableOrder, BilinearForm>>; \
        return tag_dispatch::to<clifford_t, tag_of_t<clifford_t>>( std::move( polynomial ) ); \
    } \
};

#define TAGD_DEFINE_CLIFFORD_CONCEPT_FORWARDING_FUNCTION( function, concept ) \
template<class Variable, class TotalVariableOrder, class BilinearForm, class StructureTag> \
struct function< \
    quotient_tag<models::free_polynomial_tag<Coefficient, Variable>, clifford_tag<TotalVariableOrder, BilinearForm>>, \
    concepts::concept<quotient_tag<models::free_polynomial_tag<Coefficient, Variable>, clifford_tag<TotalVariableOrder, BilinearForm>>, StructureTag> \
> { \
public: \
    template<class ...Args> static constexpr decltype(auto) apply( Args &&...args ) { \
        return function< \
            models::free_polynomial_tag<Coefficient, Variable>, \
            concepts::makeable<models::free_polynomial_tag<Coefficient, Variable>, StructureTag> \
        >( std::forward<Args>( args )... ); \
    } \
};

TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( make, makeable )
TAGD_DEFINE_CLIFFORD_CONCEPT_FORWARDING_FUNCTION( equal, set )
TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( compose_assign, monoid )
TAGD_DEFINE_CLIFFORD_CONCEPT_FORWARDING_FUNCTION( is_abelian, monoid )
TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( neutral_element, monoid )
TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( inverse_in_place, group )
TAGD_DEFINE_CLIFFORD_CONCEPT_FORWARDING_FUNCTION( is_abelian, ring )
TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( multiply_assign, ring )
TAGD_DEFINE_CLIFFORD_CONCEPT_FUNCTION( one, ring )
}}

#endif //TAGD_MODELS_CLIFFORD_HPP
