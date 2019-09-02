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
template<class BilinearForm>
class spacetime_index_handler {
	struct index_getter {
		auto operator()(const gamma_matrix &gm) const { return std::make_tuple(gm.index); }
		
		auto operator()(const sigma_matrix &sm) const { return std::make_tuple(sm.index1, sm.index2); }
		
		auto operator()(const spacetime_derivative &sd) const { return std::make_tuple(sd.index); }
		
		template<class ObjectWithoutSpacetimeIndices>
		auto operator()(ObjectWithoutSpacetimeIndices &&) const { return std::make_tuple(); }
	};
	
	template<class ...Alternatives>
	auto operator()(std::variant<Alternatives...> &&v) const { return std::visit(*this, v); }
	
	struct coefficient_decomposer {
	
	};
public:
	static constexpr auto get_indices = index_getter{};
	static constexpr auto less_indices = std::less<spacetime_index>{};
	static constexpr auto decompose_coefficient = coefficient_decomposer{};
	
	template<class Coefficient> static constexpr auto compose_coefficient = coefficient_composer < Coefficient > {};
	
	static constexpr auto contract = index_contractor{};
	static constexpr auto expand = index_expander{};
};
}
}
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::gamma_matrix, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::sigma_matrix, index1, index2 )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::spacetime_derivative, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::dirac_operator )

#endif //PQUANTUM_LINEAR_OPERATORS_HPP
