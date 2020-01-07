//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_PARSING_PARSE_HPP
#define PQUANTUM_PARSING_PARSE_HPP

#include "parser_rules/rule_for_fwd.hpp"

#include <string>

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing {
template<class T, class Context>
T parse(const std::string &string, Context &&context) {
	BOOST_LOG_NAMED_SCOPE("parsing::parse()");
	logging::severity_logger logger;
	
	T t;
	auto it = string.begin();
	bool parsing_result = boost::spirit::x3::phrase_parse(it, string.end(),
														  parser_rules::rule_for<T>(std::forward<Context>(context)),
														  boost::spirit::x3::ascii::space, t);
	
	if(parsing_result == false) {
		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Cannot parse input string: \"" << string << "\"";
		error::exit_upon_error();
	}
	
	if(it != string.end()) {
		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Cannot parse end of input string: \""
															  << std::string(it, string.end()) << "\"";
	}
	
	return t;
}
}

#endif //PQUANTUM_PARSING_PARSE_HPP
