//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULE_HPP
#define PQUANTUM_PARSING_TYPE_RULE_HPP

#include <boost/spirit/home/x3.hpp>

#include <variant>




namespace PQuantum::parsing {
// Credits go to https://stackoverflow.com/users/85371/sehe
template<class T, class Expr>
static constexpr auto as( Expr &&expr ) {
	return boost::spirit::x3::rule<struct _, T>{"as"} = boost::spirit::x3::as_parser( std::forward<Expr>(expr) );
}

struct type_rule_tag;
template<class T> using type_rule = boost::spirit::x3::rule<type_rule_tag, T>;

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

template<class T>
static auto evaluate_type_rule( void ) {
	using impl = rule_for_impl<T>;
	return (boost::spirit::x3::rule<struct _, T>{ impl::name() } = impl::apply());
}
}

template<class T>
static auto rule_for( void ) {
	using impl = rule_for_impl<T>;
	return boost::spirit::x3::rule<type_rule_tag, T>{ "type_parser_wrapper" };
}

template<class T, class InputIterator>
std::variant<T, std::tuple<bool, InputIterator, InputIterator>>
parse( InputIterator begin, InputIterator end ) {
	T t;
	auto rule = rule_for<T>();
	bool parsing_result = boost::spirit::x3::phrase_parse( begin, end, rule, boost::spirit::x3::ascii::space, t );
	
	if( parsing_result && begin == end )
		return t;
	return std::make_tuple( parsing_result, begin, end );
}
}

#define PQUANTUM_PARSING_TYPE_DECLARE(type) BOOST_SPIRIT_DECLARE(PQuantum::parsing::type_rule<type>)

#define PQUANTUM_PARSING_TYPE_DEFINE(type) \
namespace boost::spirit::x3 { \
template<typename Iterator, typename Context> \
inline bool parse_rule( \
	PQuantum::parsing::type_rule<type>, \
	Iterator &first, const Iterator &last, \
	const Context &context, type &attr \
) { \
	static auto const rule_def = PQuantum::parsing::detail::evaluate_type_rule<type>(); \
	return rule_def.parse( first, last, context, unused, attr ); \
} \
}

#endif //PQUANTUM_PARSING_TYPE_RULE_HPP
