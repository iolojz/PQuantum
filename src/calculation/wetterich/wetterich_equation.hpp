//
// Created by jayz on 14.04.20.
//

#ifndef PQUANTUM_CALCULATION_WETTERICH_WETTERICH_EQUATION_HPP
#define PQUANTUM_CALCULATION_WETTERICH_WETTERICH_EQUATION_HPP

#include <map>
#include <set>
#include <functional>

#include "../../model/lagrangian.hpp"

namespace PQuantum {
namespace calculation::wetterich {
std::map<std::size_t, model::lagrangian_tree> trace_terms_of_degrees(
	const model::lagrangian_tree &lagrangian,
	std::set<support::uuid> field_ids,
	std::set<std::size_t> field_degrees,
	const std::function<support::uuid(void)> &uuid_gen
);
}
}

#endif //PQUANTUM_CALCULATION_WETTERICH_WETTERICH_EQUATION_HPP
