//
// Created by jayz on 15.01.20.
//

#ifndef PQUANTUM_SUPPORT_PARSING_FWD_HPP
#define PQUANTUM_SUPPORT_PARSING_FWD_HPP

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::support::parsing {
namespace detail {
BOOST_TTI_HAS_TYPE( attribute_type )
}

template<class T, class = void>
struct rule_for_impl;

template<class T, class ...Args>
static auto rule_for( Args &&...args ) {
	using impl = rule_for_impl<T>;
	static constexpr bool has_specified_attribute = detail::has_type_attribute_type<impl>::value;
	
	if constexpr( has_specified_attribute ) {
		using attribute_type = typename impl::attribute_type;
		return ( boost::spirit::x3::rule<struct _, attribute_type>{ impl::name } = impl::apply(
			std::forward<Args>( args )...
		));
	} else
		return ( boost::spirit::x3::rule<struct _, T>{ impl::name } = impl::apply(
			std::forward<Args>( args )...
		));
}

namespace detail {
template<class T>
struct lazy;
}

template<class T>
static auto lazy_rule_for( void ) {
	using impl = rule_for_impl<T>;
	static constexpr bool has_specified_attribute = detail::has_type_attribute_type<impl>::value;
	
	if constexpr( has_specified_attribute ) {
		using attribute_type = typename impl::attribute_type;
		return boost::spirit::x3::rule<detail::lazy<T>, attribute_type>{ "lazy" };
	} else
		return boost::spirit::x3::rule<detail::lazy<T>, T>{ "lazy" };
	
}
}

namespace boost::spirit::x3 {
template<typename Iterator, typename Context, typename Attribute, class LazyT, class LazyAttribute>
inline bool parse_rule(
	rule<PQuantum::support::parsing::detail::lazy<LazyT>, LazyAttribute>,
	Iterator &first, const Iterator &last,
	const Context &context, Attribute &attr
) {
	static auto const rule_def = PQuantum::support::parsing::rule_for<LazyT>();
	return rule_def.parse( first, last, context, unused, attr );
}
}

#endif //PQUANTUM_SUPPORT_PARSING_FWD_HPP
