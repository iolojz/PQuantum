//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_SUPPORT_PARSING_HPP
#define PQUANTUM_SUPPORT_PARSING_HPP

#include "parsing_fwd.hpp"

namespace PQuantum::support::parsing {
namespace detail {
template<class InputIterator>
struct printable_range_wrapper {
	InputIterator begin, end;
};

template<class InputIterator>
auto make_printable_range_wrapper( InputIterator begin, InputIterator end ) {
	return printable_range_wrapper<InputIterator>{ begin, end };
}

template<class InputIterator>
std::ostream &operator<<( std::ostream &os, printable_range_wrapper<InputIterator> pw ) {
	if( pw.begin != pw.end ) {
		std::ostream &next_os = ( os << *pw.begin );
		return next_os << make_printable_range_wrapper( ++( pw.begin ), pw.end );
	}
	
	return os;
}
}

template<class T, class InputIterator, class HanaMap>
T parse( InputIterator begin, InputIterator end, HanaMap ) {
	BOOST_LOG_NAMED_SCOPE( "parsing::parse_tree()" );
	logging::severity_logger logger;
	
	using boost::spirit::x3::with;
	
	T t;
	auto rule = rule_for<T>();
	bool parsing_result = boost::spirit::x3::phrase_parse( begin, end, rule, boost::spirit::x3::ascii::space, t );
	
	if( parsing_result == false ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse input string: \"" <<
																detail::make_printable_range_wrapper( begin, end )
																<< "\"";
		
		error::exit_upon_error();
	}
	
	if( begin != end ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse end of input string: \"" <<
																detail::make_printable_range_wrapper( begin, end )
																<< "\"";
		
		error::exit_upon_error();
	}
	
	return t;
}
}

#endif //PQUANTUM_SUPPORT_PARSING_HPP
