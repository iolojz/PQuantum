//
// Created by jayz on 28.08.19.
//

#ifndef PQUANTUM_CLIFFORD_HPP
#define PQUANTUM_CLIFFORD_HPP

#include "mathutils/abstract_algebra/free_polynomial.hpp"

#include <algorithm>
#include <functional>

namespace PQuantum {
namespace mathutils {
template<class Variable, class Coefficient, class BilinearForm, class LessVariables = std::less<Variable>, class CoefficientRingTag = void>
class clifford_quotient {
	using variable = Variable;
	using coefficient = Coefficient;
	using less_variables = LessVariables;
	using coefficient_ring_tag = CoefficientRingTag;
	
	template<class V, class C, class Less, class CRTag> using quotient_alias = clifford_quotient<V, C, BilinearForm, Less, CRTag>;
	using polynomial = free_polynomial<variable, coefficient, less_variables>;
	
	friend polynomial;
	
	static bool is_canonical(const std::vector<Variable> &v) {
		return (std::adjacent_find(v.begin(), v.end(), std::not_fn(LessVariables{})) == v.end());
	}
	
	static polynomial canonicalize(polynomial::monomial &&monomial) {
		auto &&variables = monomial.variables;
		auto last_ordered = std::adjacent_find(variables.begin(), variables.end(), std::not_fn(LessVariables{}));
		
		if(last_ordered == variables.end())
			return {std::move(monomial.coefficient, std::make_move_iterator(variables.begin()),
							  std::make_move_iterator(variable.end()))};
		
		auto clifford_coefficient = BilinearForm{}(*last_ordered, *(last_ordered + 1));
		if(*last_ordered == *(last_ordered + 1)) {
			auto shortened_coefficient = std::move(monomial.coefficient) * std::move(clifford_coefficient);
			variables.erase(last_ordered, last_ordered + 1);
			
			return canonicalize({std::move(variables), std::move(shortened_coefficient)});
		}
		
		auto permuted_coefficient = -monomial.coefficient;
		std::swap(*last_ordered, *(last_ordered + 1));
		auto canonicalized_polynomial = canonicalize({variables, std::move(permuted_coefficient)});
		
		auto two = Coefficient::one() + Coefficient::one();
		auto shortened_coefficient = two * std::move(monomial.coefficient) * std::move(clifford_coefficient);
		variables.erase(last_ordered, last_ordered + 1);
		canonicalized_polynomial += canonicalize({std::move(variables), std::move(shortened_coefficient)});
		
		return std::move(canonicalized_polynomial);
	}
	
	void canonicalize(polynomial &p) {
		std::vector<std::pair<decltype(p.monomial_map)>::iterator, polynomial> canonicalized;
		
		for(auto it = monomial_map.begin(); it != monomial_map.end(); ++it) {
			if(is_canonical(it->first) == false)
				canonicalized.emplace_back(it, canonicalize({std::move(it->first), std::move(it->second)}));
		}
		
		for(const auto &c : canonicalized)
			p.monomial_map.erase(c.first);
		for(auto &c : canonicalized)
			p += std::move(c.second);
	}

public:
	using clifford = free_polynomial_quotient<variable, coefficient, quotient_alias, less_variables>;
};
}
}

#undef PQUANTUM_CLIFFORD_BOUND_OPERATOR_OVERLOAD

#endif //PQUANTUM_CLIFFORD_HPP
