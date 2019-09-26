//
// Created by jayz on 29.08.19.
//

#ifndef TAGD_CORE_ERASE_IF_HPP
#define TAGD_CORE_ERASE_IF_HPP

#ifndef __has_cpp_attribute
#define TAGD_USE_CXX20_ERASE_IF 0
#elif __has_cpp_attribute(__cpp_lib_erase_if)
#define TAGD_USE_CXX20_ERASE_IF 1
#else
#define TAGD_USE_CXX20_ERASE_IF 0
#endif

namespace tag_dispatch {
#if TAGD_USE_CXX20_ERASE_IF
using std::erase_if;
#else
// This is equivalent to std::erase_if() in c++20
template<class Key, class T, class Compare, class Alloc, class Predicate>
void erase_if(std::map<Key, T, Compare, Alloc> &c, Predicate predicate) {
	auto it = c.begin();
	auto end = c.end();
	
	while(it != end) {
		if(predicate(*it))
			it = c.erase(it);
		else
			++it;
	}
}

#endif
}

#endif //TAGD_CORE_ERASE_IF_HPP
