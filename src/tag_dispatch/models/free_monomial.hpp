//
// Created by jayz on 15.08.19.
//

#ifndef TAGD_MODELS_FREE_MONOMIAL_HPP
#define TAGD_MODELS_FREE_MONOMIAL_HPP

#include "tag_dispatch/concepts/makeable.hpp"
#include "tag_dispatch/concepts/monoid.hpp"

#include <vector>

namespace tag_dispatch {
namespace models {
template<class Coefficient, class Variable>
struct free_monomial_tag {
};

template<class Coefficient, class Variable>
struct free_monomial {
	using dispatch_tag = free_monomial_tag<Coefficient, Variable>;
	
	Coefficient coefficient;
	std::vector<Variable> variables;
	
	template<class C, TAGD_DISABLE_IF_TAG_IS(C, dispatch_tag), class ...V>
	free_monomial(C &&c, V &&...v)
			: coefficient{std::forward<C>(c)}, variables{std::forward<V>(v)...} {}
	
	free_monomial(const free_monomial &) = default;
	
	free_monomial(free_monomial &&) = default;
};
}

namespace impl {
template<class Coefficient, class Variable>
struct make<models::free_monomial_tag<Coefficient, Variable>, concepts::makeable<models::free_monomial_tag<Coefficient, Variable>, void>> {
	static models::free_monomial<Coefficient, Variable>
	apply(const models::free_monomial<Coefficient, Variable> &m) { return {m}; }
	
	static models::free_monomial<Coefficient, Variable>
	apply(models::free_monomial<Coefficient, Variable> &&m) { return {std::move(m)}; }
	
	template<class ...Args>
	static models::free_monomial<Coefficient, Variable> apply(Args &&...args) { return {std::forward<Args>(args)...}; }
};

template<class Coefficient, class Variable>
struct is_abelian<models::free_monomial_tag<Coefficient, Variable>, concepts::monoid<models::free_monomial_tag<Coefficient, Variable>, void>> {
	static constexpr bool apply(void) { return false; }
};

template<class Coefficient, class Variable>
struct compose_assign<models::free_monomial_tag<Coefficient, Variable>, concepts::monoid<models::free_monomial_tag<Coefficient, Variable>, void>> {
	static constexpr models::free_monomial<Coefficient, Variable> &
	apply(models::free_monomial<Coefficient, Variable> &m1, const models::free_monomial<Coefficient, Variable> &m2) {
		tag_dispatch::multiply_assign<>(m1.coefficient, m2.coefficient);
		m1.variables.insert(m1.variables.end(), m2.variables.begin(), m2.variables.end());
		return m1;
	}
	
	static constexpr models::free_monomial<Coefficient, Variable> &
	apply(models::free_monomial<Coefficient, Variable> &m1, models::free_monomial<Coefficient, Variable> &&m2) {
		tag_dispatch::multiply_assign<>(m1.coefficient, std::move(m2.coefficient));
		m1.variables.insert(m1.variables.end(), std::make_move_iterator(m2.variables.begin()),
							std::make_move_iterator(m2.variables.end()));
		return m1;
	}
	
	static constexpr models::free_monomial<Coefficient, Variable>
	apply(models::free_monomial<Coefficient, Variable> &&m1, const models::free_monomial<Coefficient, Variable> &m2) {
		tag_dispatch::multiply_assign<>(m1.coefficient, std::move(m2.coefficient));
		m1.variables.insert(m1.variables.end(), m2.variables.begin(), m2.variables.end());
		return m1;
	}
	
	static constexpr models::free_monomial<Coefficient, Variable>
	apply(models::free_monomial<Coefficient, Variable> &&m1, models::free_monomial<Coefficient, Variable> &&m2) {
		tag_dispatch::multiply_assign<>(m1.coefficient, std::move(m2.coefficient));
		m1.variables.insert(m1.variables.end(), std::make_move_iterator(m2.variables.begin()),
							std::make_move_iterator(m2.variables.end()));
		return m1;
	}
};

template<class Coefficient, class Variable>
struct neutral_element<models::free_monomial_tag<Coefficient, Variable>, concepts::monoid<models::free_monomial_tag<Coefficient, Variable>, void>> {
	static constexpr auto apply(void) {
		return models::free_monomial<Coefficient, Variable>{tag_dispatch::one<tag_of_t < Coefficient>>()};
	}
};
}

template<class Coefficient, class Variable, class StructureTag = void> static constexpr auto make_free_monomial = make<models::free_monomial_tag<Coefficient, Variable>, StructureTag>;
}

#endif //TAGD_MODELS_FREE_MONOMIAL_HPP
