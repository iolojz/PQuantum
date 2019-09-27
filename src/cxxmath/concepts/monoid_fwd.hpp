//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_MONOID_FWD_HPP
#define CXXMATH_CONCEPTS_MONOID_FWD_HPP

namespace cxxmath
{
namespace concepts
{
template<class Compose, class NeutralElement, class IsAbelian, bool Assignable = false>
struct monoid;
}

template<class Tag>
struct default_monoid;
}

#endif //CXXMATH_CONCEPTS_MONOID_FWD_HPP
