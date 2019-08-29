//
// Created by jayz on 29.08.19.
//

#ifndef PQUANTUM_GROUP_HPP
#define PQUANTUM_GROUP_HPP

namespace PQuantum {
namespace mathutils {
namespace abstract_algebra {
template<class GroupElementType, class Tag = void>
struct group;

// group of integers under addition (no overflow detection)
template<>
struct group<int, void> {
	static constexpr bool is_abelian = true;
	
	static constexpr int compose(int a, int b) noexcept { return a + b; }
	
	static constexpr int inverse(int a) noexcept { return -a; }
	
	static constexpr int neutral_element(void) noexcept { return 0; }
};
}
}
}

#endif //PQUANTUM_GROUP_HPP
