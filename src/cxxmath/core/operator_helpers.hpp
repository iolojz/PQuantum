//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CORE_OPERATOR_HELPERS_HPP
#define CXXMATH_CORE_OPERATOR_HELPERS_HPP

#include "../concepts/logical.hpp"

namespace cxxmath
{
namespace impl
{
template<class BinaryOperatorAssign, class IsAbelian>
struct binary_operator
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return BinaryOperatorAssign::template supports_tag<Tag>();
	}
	
	template<class Arg1, class Arg2>
	static constexpr auto apply( Arg1 &&arg1, Arg2 &&arg2 )
	{
		if constexpr( std::is_rvalue_reference_v<Arg1 &&> )
			return BinaryOperatorAssign::apply( arg1, std::forward<Arg2>( arg2 ));
		else if constexpr( if_( IsAbelian::apply(), std::is_rvalue_reference_v<Arg2 &&>, false ))
			return BinaryOperatorAssign::apply( arg2, std::forward<Arg1>( arg1 ));
		else {
			auto copy = std::decay_t<Arg1>{ std::forward<Arg1>( arg1 ) };
			BinaryOperatorAssign::apply( copy, std::forward<Arg2>( arg2 ));
			return copy;
		}
	}
};

template<class BinaryOperator, class Inverse>
struct binary_operator_invert_second
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return ( Inverse::template supports_tag<Tag>() && BinaryOperator::template supports_tag<Tag>());
	}
	
	template<class Arg1, class Arg2>
	static constexpr decltype( auto ) apply( Arg1 &&arg1, Arg2 &&arg2 )
	{
		return BinaryOperator::apply( std::forward<Arg1>( arg1 ), Inverse::apply( std::forward<Arg2>( arg2 )));
	}
};

template<class UnaryOperatorAssign>
struct unary_operator
{
	template<class Tag>
	static constexpr bool supports_tag( void )
	{
		return UnaryOperatorAssign::template supports_tag<Tag>();
	}
	
	template<class Arg>
	static constexpr auto apply( Arg &&arg )
	{
		if constexpr( std::is_rvalue_reference_v<Arg &&> )
			return UnaryOperatorAssign::apply( arg );
		else {
			auto copy = std::decay_t<Arg>{ std::forward<Arg>( arg ) };
			return UnaryOperatorAssign::apply( copy );
			return copy;
		}
	}
};
}

template<class BinaryOperatorAssign, class IsAbelian> static constexpr auto binary_operator_v = function_object_v<impl::binary_operator<BinaryOperatorAssign, IsAbelian>>;
template<class BinaryOperator, class Inverse> static constexpr auto binary_operator_invert_second_v = function_object_v<impl::binary_operator_invert_second<BinaryOperator, Inverse>>;
template<class UnaryOperatorAssign> static constexpr auto unary_operator_v = function_object_v<impl::unary_operator<UnaryOperatorAssign>>;
}


#ifdef CXXMATH_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD
#error "CXXMATH_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD already defined"
#endif
#define CXXMATH_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD( classname, op ) \
classname operator op( const classname &arg ) const & \
{ return classname{ *this } op ## = arg; } \
\
classname operator op( const classname &arg ) && \
{ return std::move( *this ) op ## = arg; } \
\
classname operator op( classname &&arg ) const & \
{ return std::move( arg ) op ## = *this; } \
\
classname operator op( classname &&arg ) && \
{ return std::move( *this ) op ## = std::move( arg ); }

#ifdef CXXMATH_BINARY_OPERATOR_OVERLOAD
#error "CXXMATH_BINARY_OPERATOR_OVERLOAD already defined"
#endif
#define CXXMATH_BINARY_OPERATOR_OVERLOAD( classname, op ) \
classname operator op( const classname &arg ) const & \
{ return classname{ *this } op ## = arg; } \
\
classname operator op( const classname &arg ) && \
{ return std::move( *this ) op ## = arg; } \
\
classname operator op( classname &&arg ) const & \
{ return classname{ *this } op ## = std::move( arg ); } \
\
classname operator op( classname &&arg ) && \
{ return std::move( *this ) op ## = std::move( arg ); }

#endif //CXXMATH_CORE_OPERATOR_HELPERS_HPP
