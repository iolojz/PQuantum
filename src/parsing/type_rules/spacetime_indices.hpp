//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_SPACETIME_INDICES_HPP
#define PQUANTUM_PARSING_TYPE_RULES_SPACETIME_INDICES_HPP

namespace PQuantum::parsing::type_rules {
namespace detail {
struct index_core;
}

template<>
struct rule_for_impl<detail::index_core> {
	using attribute_type = std::variant<int, boost::uuids::uuid>;
	static constexpr std::string_view name = "index_core";
	
	template<class PContext>
	static auto operator()( PContext pcontext ) {
		auto index_uuid_gen = pcontext.external_context.index_id_lookup_and_generate;
		return ( rule_for<int>( lp_context ) | rule_for<string_id>( index_uuid_gen )).operator[](
			[]( auto &&x3_context ) {
				boost::spirit::x3::_val( x3_context ) = support::to_std_variant<int, boost::uuids::uuid>(
					boost::spirit::x3::_attr( x3_context ));
			} );
	}
};

template<>
struct rule_for_impl<mathutils::spacetime_index> {
	static constexpr std::string_view name = "spacetime_index";
	
	template<class Context>
	auto operator()( Context context ) const {
		const auto variance_def = boost::spirit::x3::lit( "_" ).operator[]( []( auto &&x3_context ) {
			boost::spirit::x3::_val( x3_context ).variance = mathutils::spacetime_index::index_variance::lower;
		} ) | boost::spirit::x3::lit( "^" ).operator[]( []( auto &&x3_context ) {
			boost::spirit::x3::_val( x3_context ).variance = mathutils::spacetime_index::index_variance::upper;
		} );
		
		auto index_core_def = rule_for<index_core>( context );
		return (( boost::spirit::x3::lit( "{" ) >> index_core_def >> boost::spirit::x3::lit( "}" )) | index_core_def );
	}
};
}

#endif //PQUANTUM_PARSING_TYPE_RULES_SPACETIME_INDICES_HPP
