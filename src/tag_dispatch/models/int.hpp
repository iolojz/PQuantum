//
// Created by jayz on 09.09.19.
//

#ifndef TAGD_MODELS_BOOL_HPP
#define TAGD_MODELS_BOOL_HPP

#include "tag_dispatch/concepts/ring.hpp"

namespace tag_dispatch::impl {
// Concrete model: default int
template<>
struct make<int, concepts::makeable<int, void>> {
	static constexpr int apply(int a) { return a; }
};

template<>
struct is_abelian<int, concepts::group<int, void>> {
	static constexpr bool apply(void) { return true; }
};

template<>
struct compose_assign<int, concepts::group<int, void>> {
	static constexpr int &apply(int &a, int b) { return a += b; }
	
	static constexpr int apply(int &&a, int b) { return a + b; }
};

template<>
struct inverse_in_place<int, concepts::group<int, void>> {
	static constexpr int &apply(int &a) { return (a = -a); }
	
	static constexpr int apply(int &&a) { return -a; }
};

template<>
struct neutral_element<int, concepts::group<int, void>> {
	static constexpr int apply(void) { return 0; }
};

template<>
struct is_abelian<int, concepts::ring<int, void>> {
	static constexpr bool apply(void) { return true; }
};

template<>
struct multiply_assign<int, concepts::ring<int, void>> {
	static constexpr int &apply(int &a, int b) { return a *= b; }
	
	static constexpr int apply(int &&a, int b) { return a * b; }
};

template<>
struct one<int, concepts::ring<int, void>> {
	static constexpr bool apply(void) { return 1; }
};

template<>
struct less<int, concepts::total_order<int, void>> {
	static constexpr bool apply(int a, int b) { return (a < b); }
};
}

#endif // TAGD_MODELS_BOOL_HPP
