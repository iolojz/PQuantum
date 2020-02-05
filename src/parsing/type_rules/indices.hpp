//
// Created by jayz on 05.02.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_INDICES_HPP
#define PQUANTUM_PARSING_TYPE_RULES_INDICES_HPP

namespace PQuantum::parsing {
struct spacetime_index_core {};
struct index_id {};

template<> struct attribute<spacetime_index_core> {
	using type = std::variant<int, boost::uuids::uuid>;
};

template<> struct attribute<index_id> {
	using type = boost::uuids::uuid;
};

struct type_rule_impl<index_id> {
	static auto apply( void ) {
		return make_type_rule<identifier>().operator[]( [] ( auto &&x3_context ) {
			auto index_id_lookup_and_generate = boost::spirit::x3::get<index_id_lookup_and_generate_tag>( x3_context );
			boost::spirit::attr_(x3_context) = index_id_lookup_and_generate(
				boost::spirit::val_(x3_context)
			);
		} );
	}
};

struct type_rule_impl<mathutils::linear_operators::spacetime_index::index_variance> {
	static auto apply( void ) {
		constexpr auto lower = mathutils::linear_operators::spacetime_index::index_variance::lower;
		constexpr auto upper = mathutils::linear_operators::spacetime_index::index_variance::upper;
		using boost::spirit::x3::attr;
		
		return ((attr(lower) >> '_') | (attr(upper) >> '^'));
	}
};

struct type_rule_impl<spacetime_index_core> {
	static auto apply( void ) {
		using boost::spirit::x3::int_;
		auto id_rule = make_type_rule<index_id, boost::uuids::uuid>();
		
		return '{' >> (int_ | id_rule) >> '}'.operator[]( [] ( auto &&x3_context ) {
			boost::spirit::attr_(x3_context) = to_std_variant<int, boost::uuids::uuid>(
				boost::spirit::val_(x3_context)
			);
		} );
	}
};

template<class TreeTag>
struct type_rule_impl<support::tree::node_incarnation<mathutils::linear_operators::spacetime_index, TreeTag>> {
	static auto apply( void ) {
		using mathutils::linear_operators::spacetime_index::index_variance;
		auto core_rule = make_type_rule<spacetime_index_core, std::variant<int, boost::uuids::uuid>>();
		
		return make_type_rule<index_variance>() >> core_rule;
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_INDICES_HPP
