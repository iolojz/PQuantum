//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_STRING_ID_HPP
#define PQUANTUM_IO_STRING_ID_HPP

#include "rule_for_symbol_fwd.hpp"

#include <string>

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::io::parser_rules {
struct string_id;

template<>
struct rule_for_symbol_impl<string_id> {
	template<class StringToUUID>
	auto operator()(StringToUUID &&uuid_gen) const {
		auto rule_def = (+boost::spirit::x3::char_).operator[]([&uuid_gen](auto &&context) {
			boost::spirit::x3::_val(context) = uuid_gen(boost::spirit::x3::_attr(context));
		});
		return (boost::spirit::x3::rule<string_id, boost::uuids::uuid>{} = rule_def);
	}
};
}

#endif //PQUANTUM_IO_STRING_ID_HPP
