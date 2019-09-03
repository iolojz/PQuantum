//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_LINEAR_OPERATORS_HPP
#define PQUANTUM_LINEAR_OPERATORS_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include "spacetime_index.hpp"

namespace PQuantum
{
namespace mathutils
{
struct gamma_matrix
{
	spacetime_index index;
	
	constexpr bool operator<( const gamma_matrix &gm ) const
	{
		return index < gm.index;
	}
};

static inline std::ostream &operator<<( std::ostream &os, const gamma_matrix &gm )
{
	return os << "\\gamma" << gm.index;
}

struct sigma_matrix
{
	spacetime_index index1, index2;
	
	constexpr bool operator<( const sigma_matrix &sm ) const
	{
		if( index1 < sm.index1 )
			return true;
		if( index1 == sm.index1 )
			return index2 < sm.index2;
		
		return false;
	}
};

struct spacetime_derivative
{
	spacetime_index index;
	
	constexpr bool operator<( const spacetime_derivative &sd ) const
	{
		return index < sd.index;
	}
};

struct dirac_operator
{
	constexpr bool operator<( const dirac_operator & ) const
	{
		return false;
	}
};

using linear_operator = std::variant<gamma_matrix, sigma_matrix, spacetime_derivative, dirac_operator>;

namespace abstract_algebra {
namespace detail
{
template<class BilinearForm, class Indices>
struct spacetime_index_contractor
{
	Indices indices;
	
	template<class Arg1, class Arg2>
	auto operator()( Arg1 &&a1, Arg2 &&a2 ) const
	{ return BilinearForm{}( std::forward<Arg1>( a1 ), std::forward<Arg2>( a2 )); }
};
}

template<class BilinearForm>
class spacetime_index_handler {
	struct index_getter {
		auto operator()(const gamma_matrix &gm) const { return std::make_tuple(gm.index); }
		
		auto operator()(const sigma_matrix &sm) const { return std::make_tuple(sm.index1, sm.index2); }
		
		auto operator()(const spacetime_derivative &sd) const { return std::make_tuple(sd.index); }
		
		template<class ...Alternatives>
		auto operator()( std::variant<Alternatives...> &&v ) const
		{ return std::visit( *this, v ); }
		
		template<class ObjectWithoutSpacetimeIndices>
		auto operator()(ObjectWithoutSpacetimeIndices &&) const { return std::make_tuple(); }
	};
	
	template<class FreePolynomial>
	struct index_contractor
	{
		static_assert( std::is_same_v<type_dispatch::tag_of_t < FreePolynomial>, type_dispatch::free_polynomial_tag > ,
					   "FreePolynomial must be a free_polynomial<>." );
		
		template<class CPartRangePre, class CPart, class CPartRangePost, class Variable, class Indices>
		auto
		operator()( CPartRangePre &&cpre, CPart &&c, CPartRangePost &&cpost, Variable &&v, Indices &&indices ) const
		{
		
		}
		
		template<class Variable1, class Variable2, class Indices, class =
		std::enable_if_t<std::is_same_v<type_dispatch::tag_of_t < Variable1>, type_dispatch::std_variant_tag> &&
		std::is_same_v<type_dispatch::tag_of_t < Variable2>, type_dispatch::std_variant_tag>
		>>
		
		auto operator()( Variable1 &&v1, Variable2 &&v2, Indices &&indices ) const
		{
			auto contractor = spacetime_index_contractor < std::decay_t<Indices>>
			{ std::forward<Indices>( indices ) };
			return std::visit( contractor, std::forward<Variable1>( v1 ), std::forward<Variable2>( v2 ))
		}
		
		template<class Variable1, class VariableRange, class Variable2, class Indices>
		auto operator()( Variable1 &&v1, VariableRange &&middle, Variable2 &&v2, Indices &&indices ) const
		{
			if( std::size( middle ) != 0 ) {
				std::vector<typename FreePolynomial::variable> variables{ std::forward<Variable1>( v1 ) };
				variables.insert( variables.end(), template_helpers::begin( std::forward<VariableRange>( middle )),
								  template_helpers::end( std::forward<VariableRange>( middle )));
				variables.push_back( std::forward<Variable2>( v2 ));
				
				return FreePolynomial{ FreePolynomial::coefficient_ring::one(),
									   { std::make_move_iterator( variables.begin()),
										 std::make_move_iterator( variables.end()) }};
			}
			
			return ( *this )( std::forward<Variable1>( v1 ), std::forward<Variable2>( v2 ),
							  std::forward<Indices>( indices ));
		}
	};
public:
	static constexpr auto get_indices = index_getter{};
	static constexpr auto less_indices = std::less<spacetime_index>{};
	
	static constexpr auto decompose_coefficient = detail::default_coefficient_decomposer{};
	template<class Coefficient> static constexpr auto compose_coefficient = detail::default_coefficient_composer<Coefficient>{};
	
	template<class FreePolynomial> static constexpr auto contract = index_contractor<FreePolynomial>{};
	template<class FreePolynomial> static constexpr auto expand = index_expander < FreePolynomial > {};
};
}
}
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::gamma_matrix, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::sigma_matrix, index1, index2 )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::spacetime_derivative, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::dirac_operator )

#endif //PQUANTUM_LINEAR_OPERATORS_HPP
