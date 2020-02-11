//
// Created by jayz on 05.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_PARAMETER_HPP
#define PQUANTUM_PARSING_TYPE_RULES_PARAMETER_HPP

namespace PQuantum::parsing {
struct type_rule_impl<variable_id> {
	static auto apply( void ) {
		return make_type_rule<identifier>().operator[]( [] ( auto &&x3_context ) {
			auto parameter_id_lookup = boost::spirit::x3::get<parameter_id_lookup_tag>( x3_context );
			boost::spirit::attr_(x3_context) = parameter_id_lookup( boost::spirit::val_(x3_context) );
		} );
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_PARAMETER_HPP
