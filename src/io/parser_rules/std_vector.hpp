//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_STD_VECTOR_HPP
#define PQUANTUM_IO_STD_VECTOR_HPP

#include "rule_for_symbol_fwd.hpp"

#include <vector>

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::io::parser_rules {
template<class Symbol>
struct rule_for_symbol_impl<std::vector<Symbol>> {
	template<class Context>
	auto operator()(Context &&context) const {
		auto rule_def = *rule_for_symbol<Symbol>(std::forward<Context>(context));
		return (boost::spirit::x3::rule<std::vector<Symbol>, std::vector<Symbol>>{} = rule_def);
	}
};
}

#endif //PQUANTUM_IO_STD_VECTOR_HPP
