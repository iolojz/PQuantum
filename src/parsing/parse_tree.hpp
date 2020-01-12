//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_PARSING_PARSE_HPP
#define PQUANTUM_PARSING_PARSE_HPP

#include "parsing/parser_rules/node_rule_for_fwd.hpp"

#include <string>

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing {
struct pquantum_context;
template<class NodeData, class InputIterator, class Context>
tree_node<NodeData> parse_tree( InputIterator begin, InputIterator end, Context context )
{
	BOOST_LOG_NAMED_SCOPE( "parsing::parse_tree()" );
	logging::severity_logger logger;
	
	using boost::spirit::x3::with;
	using node_type = tree_node<NodeData>;
	template<class Data> using node_rule_for = parser_rules::node_rule_for<node_type, Data>;
	
	tree_node <NodeData> root;
	bool parsing_result = boost::spirit::x3::phrase_parse( begin, end, with<pquantum_context>(
	context )[parser_rules::node_rule_for<NodeData>( context )], boost::spirit::x3::ascii::space, root );
	
	if( parsing_result == false ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse input string: \""
																<< std::string( it, end ) << "\"";
		error::exit_upon_error();
	}
	
	if(it != string.end()) {
		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Cannot parse end of input string: \""
															  << std::string( it, end ) << "\"";
		error::exit_upon_error();
	}
	
	return t;
}

template<class T, class Context>
T parse(const std::string &string, Context context) {
	using boost::spirit::x3::with;
	
	BOOST_LOG_NAMED_SCOPE("parsing::parse()");
	logging::severity_logger logger;
	
	T t;
	auto it = string.begin();
	bool parsing_result = boost::spirit::x3::phrase_parse(
		it, string.end(),
		with<pquantum_context>( context )[parser_rules::rule_for<T>( context )],
		boost::spirit::x3::ascii::space, t
	);
	
	if(parsing_result == false) {
		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Cannot parse input string: \"" << string << "\"";
		error::exit_upon_error();
	}
	
	if(it != string.end()) {
		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Cannot parse end of input string: \""
															  << std::string(it, string.end()) << "\"";
		error::exit_upon_error();
	}
	
	return t;
}
}

#endif //PQUANTUM_PARSING_PARSE_HPP
