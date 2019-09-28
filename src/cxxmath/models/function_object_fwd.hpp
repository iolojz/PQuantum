//
// Created by jayz on 28.09.19.
//

#ifndef CXXMATH_MODELS_FUNCTION_OBJECT_FWD_HPP
#define CXXMATH_MODELS_FUNCTION_OBJECT_FWD_HPP

namespace cxxmath
{
namespace impl
{
struct identity;
struct unsupported_implementation;
}

struct function_object_tag;

template<class Implementation>
struct function_object;
}

#endif //CXXMATH_MODELS_FUNCTION_OBJECT_FWD_HPP
