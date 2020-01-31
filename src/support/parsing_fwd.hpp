//
// Created by jayz on 15.01.20.
//

#ifndef PQUANTUM_SUPPORT_PARSING_FWD_HPP
#define PQUANTUM_SUPPORT_PARSING_FWD_HPP

#include <boost/spirit/home/x3.hpp>
#include <variant>

namespace PQuantum::support::parsing {
template<class T, class = void>
struct rule_for_impl;

template<class T>
static auto rule_for( void );

template<class T, class InputIterator>
std::variant<T, std::tuple<bool, InputIterator, InputIterator>>
parse( InputIterator begin, InputIterator end );
}

#endif //PQUANTUM_SUPPORT_PARSING_FWD_HPP
