//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_MONOID_FWD_HPP
#define CXXMATH_CONCEPTS_MONOID_FWD_HPP

#include "../models/function_object_fwd.hpp"
#include "../core/operator_helpers_fwd.hpp"

namespace cxxmath
{
namespace concepts
{
template<class ComposeAssign, class Compose, class NeutralElement, class IsAbelian>
struct monoid;

template<class Compose, class NeutralElement, class IsAbelian> using non_assignable_monoid = monoid<impl::unsupported_implementation, Compose, NeutralElement, IsAbelian>;

template<class ComposeAssign, class NeutralElement, class IsAbelian> using assignable_monoid = monoid<ComposeAssign, impl::binary_operator<ComposeAssign, IsAbelian>, NeutralElement, IsAbelian>;
}

template<class Tag, class = std::void_t<>>
struct default_monoid;
}

#endif //CXXMATH_CONCEPTS_MONOID_FWD_HPP
