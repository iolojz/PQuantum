//
// Created by jayz on 28.09.19.
//

#ifndef CXXMATH_CORE_OPERATOR_HELPERS_FWD_HPP
#define CXXMATH_CORE_OPERATOR_HELPERS_FWD_HPP

namespace cxxmath::impl
{
template<class BinaryOperatorAssign, class IsAbelian>
struct binary_operator;

template<class BinaryOperator, class Inverse>
struct binary_operator_invert_second;

template<class UnaryOperatorAssign>
struct unary_operator;
}

#endif //CXXMATH_CORE_OPERATOR_HELPERS_FWD_HPP
