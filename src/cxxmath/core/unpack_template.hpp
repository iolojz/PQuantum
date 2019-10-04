//
// Created by jayz on 04.10.19.
//

#ifndef CXXMATH_CORE_UNPACK_TEMPLATE_HPP
#define CXXMATH_CORE_UNPACK_TEMPLATE_HPP

namespace cxxmath
{
template<class UnpackMe, template<class...> class Template>
struct unpack_template;
template<class UnpackMe, template<class...> class Template> using unpack_template_t = typename unpack_template<UnpackMe, Template>::type;

template<template<class...> class UnpackFrom,
template<class...> class PackInto, class ...Args>
struct unpack_template<UnpackFrom<Args...>, PackInto>
{
	using type = PackInto<Args...>;
};
}

#endif //CXXMATH_CORE_UNPACK_TEMPLATE_HPP
