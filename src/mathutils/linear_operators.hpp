//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_LINEAR_OPERATORS_HPP
#define PQUANTUM_LINEAR_OPERATORS_HPP

#include <boost/fusion/include/adapt_struct.hpp>

#include "spacetime_index.hpp"

namespace PQuantum::mathutils
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

using linear_operator = std::variant<imaginary_unit, gamma_matrix, sigma_matrix, spacetime_derivative, dirac_operator>;
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::gamma_matrix, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::sigma_matrix, index1, index2 )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::spacetime_derivative, index )
BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::dirac_operator )

#endif //PQUANTUM_LINEAR_OPERATORS_HPP
