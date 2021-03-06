//
// Created by jayz on 11.01.20.
//

#ifndef PQUANTUM_MODEL_PARAMETER_HPP
#define PQUANTUM_MODEL_PARAMETER_HPP

#include "support/uuid.hpp"

namespace PQuantum::model {
struct indexed_parameter {
	support::uuid id;
	mathutils::index_spec<std::variant<int, support::uuid>> indices;
	
	constexpr bool operator==( const indexed_parameter &other ) const {
		return (id == other.id && indices == other.indices);
	}
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const indexed_parameter &iparam ) {
	return os << iparam.id << iparam.indices;
}
}

#endif //PQUANTUM_MODEL_PARAMETER_HPP
