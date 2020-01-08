//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_NUMBER_HPP
#define PQUANTUM_NUMBER_HPP

#include <utility>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_complex.hpp>

namespace PQuantum
{
namespace mathutils
{
using number = boost::multiprecision::number<boost::multiprecision::complex_adaptor<boost::multiprecision::cpp_rational_backend>>;
}
}

#endif //PQUANTUM_NUMBER_HPP
