//
// Created by jayz on 02.09.19.
//

#ifndef PQUANTUM_MODEL_SPACETIME_INDEX_HPP
#define PQUANTUM_MODEL_SPACETIME_INDEX_HPP

#include <variant>

#include "support/uuid.hpp"

namespace PQuantum::model {
struct spacetime_index {
	enum class index_variance {
		lower, upper
	} variance;
	std::variant<int, support::uuid> id;
};
}

#endif //PQUANTUM_MODEL_SPACETIME_INDEX_HPP
