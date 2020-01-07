//
// Created by jayz on 26.11.19.
//

#ifndef PQUANTUM_IO_PARSER_RULES_STD_VARIANT_HPP
#define PQUANTUM_IO_PARSER_RULES_STD_VARIANT_HPP

#include "support/variant.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing::parser_rules {
template<class ...Alternatives>
struct rule_for_impl<std::variant<Alternatives...>> {
	template<class Context>
	constexpr auto operator()(Context &&c) const {
		auto rule_def = (rule_for<Alternatives>(c) | ...).operator[]([](auto &&context) {
			boost::spirit::x3::_val(context) = support::to_std_variant<Alternatives...>(
					boost::spirit::x3::_attr(context));
		});
		return (boost::spirit::x3::rule<std::variant<Alternatives...>, std::variant<Alternatives...>>{} = rule_def);
	}
};
}

#endif //PQUANTUM_IO_PARSER_RULES_STD_VARIANT_HPP
