//
// Created by jayz on 15.01.20.
//

#ifndef PQUANTUM_SUPPORT_IS_MEMBER_HPP
#define PQUANTUM_SUPPORT_IS_MEMBER_HPP

#include <tuple>

namespace PQuantum::support {
template<class T, class Tuple>
struct is_member : std::false_type {};

template<class T>
struct is_member<T, std::tuple<>> : std::false_type {};
template<class T, class ...Types>
struct is_member<T, std::tuple<T, Types...>> : std::true_type {};
template<class T, class U, class ...Types>
struct is_member<T, std::tuple<U, Types...>>
	: is_member<T, std::tuple<Types...>> {
};

template<class T, class Tuple>
static constexpr bool is_member_v = is_member<T, Tuple>::value;
}

#endif //PQUANTUM_SUPPORT_IS_MEMBER_HPP
