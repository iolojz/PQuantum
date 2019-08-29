//
// Created by jayz on 29.08.19.
//

#ifndef PQUANTUM_RING_HPP
#define PQUANTUM_RING_HPP

#include "group.hpp"

namespace PQuantum {
namespace mathutils {
class number;

namespace abstract_algebra {
template<class RingElementType, class Tag = void>
struct ring;

// ring of integers under addition and multiplication (no overflow detection)
template<>
struct ring<int, void> {
	static constexpr int add(int a, int b) noexcept { return group<int>::compose(a, b); }
	
	static constexpr int multiply(int a, int b) noexcept { return a * b; }
	
	static constexpr int negate(int a) noexcept { return group<int>::inverse(a); }
	
	static constexpr int zero(void) noexcept { return group<int>::neutral_element(); }
	
	static constexpr int one(void) noexcept { return 1; }
};
}
}
}

#endif //PQUANTUM_RING_HPP
