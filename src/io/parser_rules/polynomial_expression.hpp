//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_POLYNOMIAL_EXPRESSION_HPP
#define PQUANTUM_POLYNOMIAL_EXPRESSION_HPP

#include "free_r_algebra.hpp"
#include "string_id.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::io::parser_rules {
template<>
struct rule_for_symbol_impl<mathutils::imaginary_unit> {
	template<class Context>
	auto operator()(Context &&) const {
		auto rule_def = boost::spirit::x3::lit("\\ImaginaryUnit").operator[]([](auto &&) {});
		return (boost::spirit::x3::rule<mathutils::imaginary_unit, mathutils::imaginary_unit>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<mathutils::variable_id> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		auto rule_def = rule_for_symbol<string_id>(lp_context.parameter_id_lookup);
		return (boost::spirit::x3::rule<mathutils::variable_id, mathutils::variable_id>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<mathutils::spacetime_dimension> {
	template<class Context>
	auto operator()(Context &&) const {
		auto rule_def = boost::spirit::x3::lit("\\SpacetimeDimension").operator[]([](auto &&) {});
		return (boost::spirit::x3::rule<mathutils::spacetime_dimension, mathutils::spacetime_dimension>{} = rule_def);
	}
};
}

#include "std_variant.hpp"

namespace PQuantum::io::parser_rules {
template<>
struct rule_for_symbol_impl<mathutils::expression_symbol> {
	template<class Context>
	auto operator()(Context &&context) const {
		auto rule_def = rule_for_symbol<std::decay_t<decltype(std::declval<mathutils::expression_symbol>().value)>>(
				std::forward<Context>(context));
		return (boost::spirit::x3::rule<mathutils::expression_symbol, mathutils::expression_symbol>{} = rule_def);
	}
};
}


#endif //PQUANTUM_POLYNOMIAL_EXPRESSION_HPP
