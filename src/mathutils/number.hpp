//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_NUMBER_HPP
#define PQUANTUM_NUMBER_HPP

#include <utility>

namespace PQuantum
{
namespace mathutils
{
// TODO: implement numbers...
class number
{
public:
	static number one(void) noexcept { return {}; }
	
	number &operator+=( const number & )
	{ return *this; };
	
	number &operator*=( const number & )
	{ return *this; };
	
	number operator+( const number &n ) const &
	{ return number{ *this } += n; }
	
	number operator+( const number &n ) &&
	{ return std::move( *this ) += n; }
	
	number operator+( number &&n ) const &
	{ return std::move( n ) += *this; }
	
	number operator+( number &&n ) &&
	{ return std::move( *this ) += std::move( n ); }
	
	number operator*( const number &n ) const &
	{ return number{ *this } *= n; }
	
	number operator*( const number &n ) &&
	{ return std::move( *this ) *= n; }
	
	number operator*( number &&n ) const &
	{ return std::move( n ) *= *this; }
	
	number operator*( number &&n ) &&
	{ return std::move( *this ) *= std::move( n ); }
	
	constexpr bool operator==( const number & ) const noexcept
	{ return true; }
	
	constexpr bool operator<( const number & ) const noexcept
	{ return false; }
};
}
}

#endif //PQUANTUM_NUMBER_HPP
