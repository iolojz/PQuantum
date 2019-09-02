//
// Created by jayz on 02.09.19.
//

#ifndef PQUANTUM_SPACETIME_INDEX_HPP
#define PQUANTUM_SPACETIME_INDEX_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <variant>

#include "abstract_algebra/abstract_index_notation.hpp"

namespace PQuantum {
namespace mathutils {
struct spacetime_index {
	enum class index_variance {
		lower, upper
	} variance;
	std::variant<int, boost::uuids::uuid> id;
	
	constexpr bool operator==(const spacetime_index &si) const {
		return (id == si.id && variance == si.variance);
	}
};

static constexpr bool operator<(const spacetime_index::index_variance &v1, const spacetime_index::index_variance &v2) {
	switch(v1) {
		case spacetime_index::index_variance::lower:
			return v2 == spacetime_index::index_variance::upper;
			break;
		default:
			return false;
			break;
	}
	
	return false;
}

namespace detail {
struct less_spacetime_index_ids {
	constexpr bool operator()(int id1, int id2) const noexcept { return (id1 < id2); }
	
	constexpr bool operator()(int, boost::uuids::uuid) const noexcept { return true; }
	
	constexpr bool operator()(boost::uuids::uuid, int) const noexcept { return false; }
	
	bool operator()(boost::uuids::uuid id1, boost::uuids::uuid id2) const noexcept { return (id1 < id2); }
};
}

static constexpr bool operator<(const spacetime_index &si1, const spacetime_index &si2) {
	return std::visit(detail::less_spacetime_index_ids{}, si1.id, si2.id);
}

static inline std::ostream &operator<<(std::ostream &os, const spacetime_index &si) {
	if(si.variance == spacetime_index::index_variance::upper)
		os << "^";
	else
		os << "_";
	
	if(std::holds_alternative<int>(si.id))
		return os << std::get<int>(si.id);
	
	return os << "{\\index{" << std::get<boost::uuids::uuid>(si.id) << "}}";
}
}
}

BOOST_FUSION_ADAPT_STRUCT(PQuantum::mathutils::spacetime_index, variance, id)

#endif //PQUANTUM_SPACETIME_INDEX_HPP
