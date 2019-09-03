//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_MAKE_HPP
#define PQUANTUM_MAKE_HPP

#include "tag_dispatch.hpp"

namespace PQuantum::tag_dispatch {
namespace impl {
template<class Tag>
struct make {
	template<class ...Args>
	decltype(auto) operator()(Args &&...args) const { return Tag{std::forward<Args>(args)...}; }
};
}

PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(make);
}

#endif //PQUANTUM_MAKE_HPP
