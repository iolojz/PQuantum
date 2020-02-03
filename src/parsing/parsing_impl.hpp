//
// Created by jayz on 03.02.20.
//

#ifndef PQUANTUM_PARSING_PARSING_IMPL_HPP
#define PQUANTUM_PARSING_PARSING_IMPL_HPP

#include "error/error.hpp"
#include "logging/logging.hpp"

#include "type_rule.hpp"

namespace PQuantum::parsing {
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

template<class T, class InputIterator>
T phrase_parse_type_skip_space_error( InputIterator begin, InputIterator end ) {
	BOOST_LOG_NAMED_SCOPE( "parsing::phrase_parse_skip_space_error()" );
	logging::severity_logger logger;
	using detail::make_printable_range_wrapper;
	
	auto result = phrase_parse_type_skip_space<T>( begin, end );
	if( result.index() == 0 )
		return std::get<0>( result );
	
	auto parsing_result_tuple = std::get<1>( result );
	if( std::get<0>( parsing_result_tuple ) ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error )
			<< "Unable to parse end of input string: \""
			<< make_printable_range_wrapper( std::get<1>( parsing_result_tuple ), std::get<2>( parsing_result_tuple ) )
			<< "\"";
		error::exit_upon_error();
	}
	
	BOOST_LOG_SEV( logger, logging::severity_level::error )
		<< "unable to parse input string: \""
		<< make_printable_range_wrapper( std::get<1>( parsing_result_tuple ), std::get<2>( parsing_result_tuple ) )
		<< "\"";
	error::exit_upon_error();
}
}

#endif //PQUANTUM_PARSING_PARSING_IMPL_HPP
