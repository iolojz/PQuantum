//
// Created by jayz on 08.01.20.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_NUMBER_HPP
#define PQUANTUM_PARSING_PARSER_RULES_NUMBER_HPP

#include "linear_operators.hpp"

namespace PQuantum::parsing::parser_rules {
struct real_number;
struct imaginary_number;

template<>
struct rule_for_impl<real_number> {
	template<class Context>
	auto operator()(Context) const {
		auto rule_def = boost::spirit::x3::long_long.operator[]( [] ( auto &&x3_context ) {
			boost::spirit::x3::_val( x3_context ) = boost::spirit::x3::_attr( x3_context );
		} );
		boost::spirit::x3::rule<real_number, mathutils::number> rule{"number_core"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<imaginary_number> {
	template<class Context>
	auto operator()(Context) const {
		auto rule_def = (-boost::spirit::x3::long_long >> boost::spirit::x3::lit( "\\ImaginaryUnit" )).operator[](
			[] ( auto &&x3_context ) {
				const auto &ip_optional = boost::spirit::x3::_attr( x3_context );
				long long ip = ip_optional ? *ip_optional : 1;
				
				mathutils::number number{ 0, ip };
				boost::spirit::x3::_val( x3_context ) = std::move( number );
			}
		);
		boost::spirit::x3::rule<imaginary_number, mathutils::number> rule{"number_core"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<no_arithmetic_tag<mathutils::number>> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = (rule_for<imaginary_number>( context ) | rule_for<real_number>( context ));
		boost::spirit::x3::rule<no_arithmetic_tag<mathutils::number>, mathutils::number> rule{"number_core"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::number> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = rule_for<arithmetic_tag<mathutils::number>>( context );
		boost::spirit::x3::rule<mathutils::number, mathutils::number> rule{"number"};
		return (rule = rule_def);
	}
};
}

#endif //PQUANTUM_PARSING_PARSER_RULES_NUMBER_HPP
