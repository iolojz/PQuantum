//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_SUPPORT_PARSING_HPP
#define PQUANTUM_SUPPORT_PARSING_HPP

#include "parsing_fwd.hpp"

namespace PQuantum::support::parsing {
namespace detail {
BOOST_TTI_HAS_TYPE( attribute_type )

template<class T>
struct lazy;

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

template<class T>
static auto rule_for( void ) {
	using impl = rule_for_impl<T>;
	static constexpr bool has_specified_attribute = detail::has_type_attribute_type<impl>::value;
	
	if constexpr( has_specified_attribute ) {
		using attribute_type = typename impl::attribute_type;
		return ( boost::spirit::x3::rule<struct _, attribute_type>{ impl::name } = impl::apply());
	} else
		return ( boost::spirit::x3::rule<struct _, T>{ impl::name } = impl::apply());
}
}

template<class T>
static auto rule_for( void ) {
	using impl = rule_for_impl<T>;
	static constexpr bool has_specified_attribute = detail::has_type_attribute_type<impl>::value;
	
	if constexpr( has_specified_attribute ) {
		using attribute_type = typename impl::attribute_type;
		return boost::spirit::x3::rule<detail::lazy<T>, attribute_type>{ "lazy" };
	} else
		return boost::spirit::x3::rule<detail::lazy<T>, T>{ "lazy" };
}

template<class T, class InputIterator>
std::variant<T, std::tuple<bool, InputIterator, InputIterator>>
parse( InputIterator begin, InputIterator end ) {
	using boost::spirit::x3::with;
	
	T t;
	auto rule = rule_for<T>();
	bool parsing_result = boost::spirit::x3::phrase_parse( begin, end, rule, boost::spirit::x3::ascii::space, t );
	
	if( parsing_result && begin == end )
		return t;
	return std::make_tuple( parsing_result, begin, end );
}
}

namespace boost::spirit::x3 {
template<typename Iterator, typename Context, typename Attribute, class LazyT, class LazyAttribute>
inline bool parse_rule(
	rule<PQuantum::support::parsing::detail::lazy<LazyT>, LazyAttribute>,
	Iterator &first, const Iterator &last,
	const Context &context, Attribute &attr
) {
	static auto const rule_def = PQuantum::support::parsing::detail::rule_for<LazyT>();
	return rule_def.parse( first, last, context, unused, attr );
}
}

#endif //PQUANTUM_SUPPORT_PARSING_HPP
