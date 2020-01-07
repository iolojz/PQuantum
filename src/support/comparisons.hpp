//
// Created by jayz on 28.08.19.
//

#ifndef PQUANTUM_COMPARISONS_HPP
#define PQUANTUM_COMPARISONS_HPP

namespace PQuantum::support {
template<class T, class U, class Less>
static bool equal(const T &t, const U &u, Less less) {
	return (!less(t, u) && !less(u, t));
}

namespace detail {
template<class Less>
struct less_to_equal {
	static constexpr Less less;
	
	template<class T, class U>
	auto operator()(T &&t, U &&u) const { return equal(std::forward<T>(t), std::forward<U>(u), less); }
};
}

template<class Less>
static constexpr auto less_to_equal(Less &&less) { return detail::less_to_equal{std::forward<Less>(less)}; }
}

#endif //PQUANTUM_COMPARISONS_HPP
