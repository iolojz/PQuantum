//
// Created by jayz on 28.08.19.
//

#ifndef TAGD_CORE_OPERATOR_HELPERS_HPP
#define TAGD_CORE_OPERATOR_HELPERS_HPP

#ifdef TAGD_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD
#error "TAGD_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD already defined"
#endif
#define TAGD_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(classname, op) \
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

#ifdef TAGD_BINARY_OPERATOR_OVERLOAD
#error "TAGD_BINARY_OPERATOR_OVERLOAD already defined"
#endif
#define TAGD_BINARY_OPERATOR_OVERLOAD(classname, op) \
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

#endif //TAGD_CORE_OPERATOR_HELPERS_HPP
