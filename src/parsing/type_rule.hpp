//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULE_HPP
#define PQUANTUM_PARSING_TYPE_RULE_HPP

#include <boost/spirit/home/x3.hpp>

#include <boost/type_index/ctti_type_index.hpp>

#include <boost/hana.hpp>

#include <variant>

#include "support/bool_v.hpp"

namespace PQuantum::parsing {
// Credits go to https://stackoverflow.com/users/85371/sehe
template<class T, class Expr>
static constexpr auto as( Expr &&expr ) {
	return boost::spirit::x3::rule<struct _, T>{"as"} = boost::spirit::x3::as_parser( std::forward<Expr>(expr) );
}

template<class T> std::string name_for_type_rule( void ) {
	return boost::typeindex::ctti_type_index::type_id<T>().pretty_name();
}

template<class T> auto evaluate_type_rule( boost::hana::basic_type<T>, std::enable_if_t<support::false_v<T>, int> = 42 );

struct type_rule_tag;
template<class T> using type_rule = boost::spirit::x3::rule<type_rule_tag, T>;

template<class T>
static auto lazy_rule_for( void ) {
	return boost::spirit::x3::rule<type_rule_tag, T>{ name_for_type_rule<T>().c_str() };
}

template<class T>
static auto rule_for( void ) {
	return lazy_rule_for<T>() = evaluate_type_rule( boost::hana::type_c<T> );
}

template<class T, class InputIterator>
std::variant<T, std::tuple<bool, InputIterator, InputIterator>>
phrase_parse_type_skip_space( InputIterator begin, InputIterator end ) {
	T t;
	auto rule = rule_for<T>();
	bool parsing_result = boost::spirit::x3::phrase_parse( begin, end, rule, boost::spirit::x3::ascii::space, t );
	
	if( parsing_result && begin == end )
		return t;
	return std::make_tuple( parsing_result, begin, end );
}
}

namespace boost::spirit::x3 {
template<class T, class Iterator, class Context>
inline bool parse_rule(
		PQuantum::parsing::type_rule<T>,
		Iterator &first, const Iterator &last,
		const Context &context, T &attr
) {
	static auto const rule_def = PQuantum::parsing::evaluate_type_rule( boost::hana::type_c<T> );
	return rule_def.parse( first, last, context, unused, attr );
}
}

#define PQUANTUM_PARSING_LAZY_TYPE_INSTANTIATE(type) \
namespace PQuantum::parsing { \
template<> \
auto evaluate_type_rule( \
	boost::hana::basic_type<type> \
); \
}

#endif //PQUANTUM_PARSING_TYPE_RULE_HPP
