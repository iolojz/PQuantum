//
// Created by jayz on 28.08.19.
//

#ifndef PQUANTUM_OPERATOR_HELPERS_HPP
#define PQUANTUM_OPERATOR_HELPERS_HPP

#ifdef PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD
#error "PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD already defined"
#endif

#define PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(classname, op) \
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

#ifdef PQUANTUM_BINARY_OPERATOR_OVERLOAD
#error "PQUANTUM_BINARY_OPERATOR_OVERLOAD already defined"
#endif

#define PQUANTUM_BINARY_OPERATOR_OVERLOAD(classname, op, invert) \
classname operator op( const classname &arg ) const & \
{ return classname{ *this } op ## = arg; } \
\
classname operator op( const classname &arg ) && \
{ return std::move( *this ) op ## = arg; } \
\
classname operator op( classname &&arg ) const & \
{ return invert( std::move( arg ) op ## = *this ); } \
\
classname operator op( classname &&arg ) && \
{ return std::move( *this ) op ## = std::move( arg ); }

#endif //PQUANTUM_OPERATOR_HELPERS_HPP
