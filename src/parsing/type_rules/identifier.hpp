//
// Created by jayz on 05.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
#define PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP

namespace PQuantum::parsing {
struct identifier {};

template<> struct attribute<identifier> {
	using type = std::string;
};

struct type_rule_impl<identifier> {
	static auto apply( void ) {
		return as<std::string>(
			(boost::spirit::x3::alpha | ('\\' >> boost::spirit::x3::alpha)) >> *boost::spirit::x3::alnum
		);
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_IDENTIFIER_HPP
