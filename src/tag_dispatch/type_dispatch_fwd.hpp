//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TYPE_DISPATCH_FWD_HPP
#define PQUANTUM_TYPE_DISPATCH_FWD_HPP

namespace PQuantum {
namespace type_dispatch {
template<class T>
struct tag_of {
	using type = T;
};
}
}
#endif //PQUANTUM_TYPE_DISPATCH_FWD_HPP
