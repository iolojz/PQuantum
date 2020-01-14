//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_SUPPORT_PARSING_HPP
#define PQUANTUM_SUPPORT_PARSING_HPP

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::support::parsing {
namespace detail {
template<class ForwardIterator>
struct printable_range_wrapper {
	ForwardIterator begin, end;
};

template<class ForwardIterator>
std::ostream &operator<<( std::ostream &os, printable_range_wrapper<ForwardIterator> pw ) {
	while( pw.begin != pw.end ) {
		os <<= *pw.begin;
		++( pw.begin );
	}
	
	return os;
}

BOOST_TTI_HAS_TYPE( attribute_type )
}

struct parsing_context_tag;

template<class T, class = void>
struct rule_for_impl;

template<class T, class = void> static constexpr auto rule_for = []( auto context ) {
	using impl = rule_for_impl<T>;
	static constexpr bool has_specified_attribute = detail::has_type_attribute_type<impl>::value;
	
	if constexpr( has_specified_attribute ) {
		using attribute_type = typename impl::attribute_type;
		return ( boost::spirit::x3::rule<struct _, attribute_type>{ impl::name } = impl::apply( context ));
	} else
		return ( boost::spirit::x3::rule<struct _, T>{ impl::name } = impl::apply( context ));
};

template<class ExternalContext>
struct parsing_context {
	ExternalContext external_context;
};

template<class T, class InputIterator, class ExternalContext>
T parse( InputIterator begin, InputIterator end, ExternalContext external_context ) {
	BOOST_LOG_NAMED_SCOPE( "parsing::parse_tree()" );
	logging::severity_logger logger;
	
	using boost::spirit::x3::with;
	
	T t;
	parsing_context<ExternalContext> pcontext{ external_context };
	auto rule = with<parsing_context_tag>( pcontext )[rule_for<T>( pcontext )];
	bool parsing_result = boost::spirit::x3::phrase_parse( begin, end, rule, boost::spirit::x3::ascii::space, t );
	
	if( parsing_result == false ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse input string: \"" <<
																detail::printable_range_wrapper{ begin, end } << "\"";
		
		error::exit_upon_error();
	}
	
	if( begin != end ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Cannot parse end of input string: \"" <<
																detail::printable_range_wrapper{ begin, end } << "\"";
		
		error::exit_upon_error();
	}
	
	return t;
}
}

#endif //PQUANTUM_SUPPORT_PARSING_HPP
