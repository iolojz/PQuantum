//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_LINEAR_OPERATORS_HPP
#define PQUANTUM_LINEAR_OPERATORS_HPP

#include <boost/uuid/uuid.hpp>
#include <variant>

namespace PQuantum
{
namespace mathutils
{
struct spacetime_index
{
	std::variant<int, boost::uuids::uuid> id;
	
	enum class index_variance
	{
		lower, upper
	} variance;
	
	constexpr bool operator==( const spacetime_index &si ) const
	{
		return ( id == si.id && variance == si.variance );
	}
};

constexpr bool operator<( const spacetime_index &si1, const spacetime_index &si2 )
{
	if( si1.id < si2.id )
		return true;
	if( si1.id == si2.id )
		return si1.variance < si2.variance;
	
	return false;
}

constexpr bool operator<( const spacetime_index::index_variance &v1, const spacetime_index::index_variance &v2 )
{
	switch( v1 ) {
		case spacetime_index::index_variance::lower:
			return v2 == spacetime_index::index_variance::upper;
			break;
		default:
			return false;
			break;
	}
	
	return false;
}

struct gamma_matrix
{
	spacetime_index index;
	
	constexpr bool operator<( const gamma_matrix &gm ) const
	{
		return index < gm.index;
	}
};

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
}
}

#endif //PQUANTUM_LINEAR_OPERATORS_HPP
