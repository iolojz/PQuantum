//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_STRING_ID_HPP
#define PQUANTUM_IO_STRING_ID_HPP

#include "node_rule_for_fwd.hpp"

#include "support/std_variant.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing::parser_rules {
struct string;
struct string_id;

template<>
struct rule_for_impl<string> {
	template<class Context> auto operator()( Context ) const {
		using boost::spirit::x3::char_;
		using boost::spirit::x3::alpha;
		using boost::spirit::x3::alnum;
		using boost::spirit::x3::lexeme;
		using boost::spirit::x3::repeat;
		
		auto special_chars = char_('\\') | char_('{') | char_("_") | char_("^") | char_('}');
		auto begin = (char_('\\') >> alpha) | repeat(1)[alpha];
		auto rule_def = lexeme[ begin >> *(alnum | special_chars) ];
		
		boost::spirit::x3::rule<string, std::string> rule{"string"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<string_id> {
	auto operator()( std::function<std::pair<bool, boost::uuids::uuid>(std::string)> uuid_gen ) const {
		auto string_rule = rule_for<string>( uuid_gen );
		auto rule_def = string_rule.operator[]( [uuid_gen] (auto &&x3_context) {
			const auto &str = boost::spirit::x3::_attr(x3_context);
			
			auto result = uuid_gen( str );
			if( result.first )
				boost::spirit::x3::_val(x3_context) = std::move( result.second );
			else
				boost::spirit::x3::_pass(x3_context) = false;
		});
		boost::spirit::x3::rule<string_id, boost::uuids::uuid> rule{"string_id"};
		return (rule = rule_def);
	}
};
}

#endif //PQUANTUM_IO_STRING_ID_HPP
