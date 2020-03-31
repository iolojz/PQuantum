//
// Created by jayz on 25.08.19.
//

#ifndef PQUANTUM_SUPPORT_VARIANT_HPP
#define PQUANTUM_SUPPORT_VARIANT_HPP

#include <variant>

namespace PQuantum::support {
template<class ...Alternatives>
std::ostream &operator<<( std::ostream &os, const std::variant<Alternatives...> &v ) {
	auto printer = [&os] ( const auto &x ) -> std::ostream & {
		return os << x;
	};
	return std::visit( printer, v );
}
}

#endif //PQUANTUM_SUPPORT_VARIANT_HPP
