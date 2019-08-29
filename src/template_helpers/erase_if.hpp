//
// Created by jayz on 29.08.19.
//

#ifndef PQUANTUM_ERASE_IF_HPP
#define PQUANTUM_ERASE_IF_HPP

namespace PQuantum {
namespace template_helpers {
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
}
}

#endif //PQUANTUM_ERASE_IF_HPP
