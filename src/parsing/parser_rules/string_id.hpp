//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_STRING_ID_HPP
#define PQUANTUM_IO_STRING_ID_HPP

#include "rule_for_fwd.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing::parser_rules {
struct string_id;

template<>
struct rule_for_impl<string_id> {
	auto operator()(std::function<std::pair<bool, boost::uuids::uuid>(std::string)> uuid_gen) const {
		using boost::spirit::x3::char_;
		using boost::spirit::x3::alpha;
		using boost::spirit::x3::alnum;
		using boost::spirit::x3::lexeme;
		
		auto special_chars = char_('\\') | char_('{') | char_("_") | char_("^") | char_('}');
		auto core = ((char_('\\') >> alpha) | alpha) >> *(alnum | special_chars);
		
		auto rule_def = lexeme[core].operator[]([uuid_gen](auto &&context) {
			const auto &where = boost::spirit::x3::_where(context);
			
			auto result = uuid_gen( std::string{ where.begin(), where.end() } );
			if( result.first )
				boost::spirit::x3::_val(context) = std::move( result.second );
			else
				boost::spirit::x3::_pass(context) = false;
		});
		boost::spirit::x3::rule<string_id, boost::uuids::uuid> rule{"string_id"};
		return (rule = rule_def);
	}
};
}

#endif //PQUANTUM_IO_STRING_ID_HPP
