//
// Created by jayz on 28.08.19.
//

#ifndef PQUANTUM_COMPARISONS_HPP
#define PQUANTUM_COMPARISONS_HPP

namespace PQuantum {
namespace template_helpers {
template<class T, class U, class Less>
static bool equal(const T &t, const U &u, Less less) {
	return (!less(t, u) && !less(u, t));
}
}
}

#endif //PQUANTUM_COMPARISONS_HPP
