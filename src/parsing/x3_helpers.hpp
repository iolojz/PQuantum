//
// Created by jayz on 04.02.20.
//

#ifndef PQUANTUM_PARSING_X3_HELPERS_HPP
#define PQUANTUM_PARSING_X3_HELPERS_HPP

#include <boost/hana.hpp>

#include <boost/spirit/home/x3.hpp>

#include <boost/type_index/ctti_type_index.hpp>

namespace PQuantum::parsing {
template<class T> struct type_rule_tag;
template<class T> struct attribute {
	using type = T;
};
template<class T> using attribute_t = typename atrtibute<T>::type;

template<class T> using type_rule = boost::spirit::x3::rule<type_rule_tag<T>, attribute_t<T>>;

template<class T> struct name_for_type_rule {
	static std::string apply( void ) {
		return boost::typeindex::ctti_type_index::type_id<T>().pretty_name();
	}
};

template<class T, class Attribute = T>
auto make_type_rule( void ) {
	static auto name = name_for_type_rule<T>::apply(); // BUG: STUPID x3, only stores const char *
	return type_rule<T>{ name.c_str() };
}

template<class T> struct type_rule_impl;

// Credits go to https://stackoverflow.com/users/85371/sehe
template<class T, class Expr>
auto as( Expr &&expr ) {
	static auto name = "as " + name_for_type_rule<T>::apply(); // BUG: STUPID x3, only stores const char *
	return (
		boost::spirit::x3::rule<struct _, T>{ name.c_str() } =
		boost::spirit::x3::as_parser( std::forward<Expr>(expr) )
	);
}
}

namespace boost::spirit::x3 {
template<class Iterator, class Context, class T, class Attribute>
bool parse_rule(
    PQuantum::parsing::type_rule<type_rule_tag<T>, Attribute>,
    Iterator &first, const Iterator &last,
    const Context &context, Attribute &attr
) {
	auto rule_def = (
		PQuantum::parsing::make_type_rule<T>() = PQuantum::parsing::type_rule_impl<T>::apply()
	);
	return rule_def.parse(first, last, context, unused, attr);
}
}

#endif //PQUANTUM_PARSING_X3_HELPERS_HPP
