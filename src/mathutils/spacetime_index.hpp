//
// Created by jayz on 02.09.19.
//

#ifndef PQUANTUM_MODEL_SPACETIME_INDEX_HPP
#define PQUANTUM_MODEL_SPACETIME_INDEX_HPP

#include "support/uuid.hpp"
#include "support/variant.hpp"

namespace PQuantum::mathutils {
struct spacetime_index {
	enum class index_variance {
		lower, upper
	} variance;
	std::variant<int, support::uuid> id;
	
	constexpr bool operator==( const spacetime_index &other ) const {
		return (variance == other.variance && id == other.id);
	}
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const spacetime_index::index_variance &v ) {
	switch( v ) {
	case spacetime_index::index_variance::lower:
		return os << "lower";
	case spacetime_index::index_variance::upper:
		return os << "upper";
	}
	
	throw std::runtime_error("unreachable");
	return os;
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const spacetime_index &index ) {
	switch( index.variance ) {
	case spacetime_index::index_variance::lower:
		return os << "_{" << index.id << "}";
	case spacetime_index::index_variance::upper:
		return os << "_{"<< index.id << "}";
	}
	
	throw std::runtime_error("unreachable");
	return os;
}
}

#endif //PQUANTUM_MODEL_SPACETIME_INDEX_HPP
