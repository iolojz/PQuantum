//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_HPP
#define PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_HPP

#include "support/tree.hpp"

#include "mathutils/lagrangian.hpp"

namespace PQuantum::parsing::parser_rules
{

template<std::string_view name>
struct rule_for_impl<tree_node < mathutils::node_data::sum>>
{
template<class Context>
auto operator()( Context context ) const
{
	return std::make_pair( rule_for < tree_node < lagrangian_node_data >> ( context ) % '+', "sum" );
}
};

template<std::string_view name>
struct rule_for_impl<tree_node < mathutils::node_data::difference>> {
template<class Context>
auto operator()( Context context ) const
{
	auto lnode = rule_for < tree_node < lagrangian_node_data >> ( context );
	return std::make_pair( lnode >> '-' >> lnode, "difference" );
}

};

template<std::string_view name>
struct rule_for_impl<tree_node < mathutils::node_data::product>> {
template<class Context>
auto operator()( Context context ) const
{
	return std::make_pair( rule_for < tree_node < lagrangian_node_data >> ( context ) % '+', "product" );
}

};

template<std::string_view name>
struct rule_for_impl<tree_node < mathutils::node_data::quotient>> {
template<class Context>
auto operator()( Context context ) const
{
	auto lnode = rule_for < tree_node < lagrangian_node_data >> ( context );
	return std::make_pair( lnode >> '/' >> lnode, "quotient" );
}

};

template<std::string_view name>
struct rule_for_impl<tree_node < mathutils::node_data::gamma_matrix>> {
template<class Context>
auto operator()( Context context ) const
{
	auto lnode = rule_for < tree_node < lagrangian_node_data >> ( context );
	return std::make_pair( lnode >> '/' >> lnode, "quotient" );
}

};

mathutils::node_data::gamma_matrix,
mathutils::node_data::sigma_matrix,
mathutils::node_data::spacetime_derivative,
mathutils::node_data::dirac_operator,
mathutils::node_data::spacetime_index,
classical_field_id,
variable_id
}












/*
struct index_core;

template<>
struct rule_for_impl<model::classical_field_id> {
	auto operator()(lagrangian_parsing_context lp_context) const {
		auto rule_def = rule_for<string_id>(lp_context.field_id_lookup);
		boost::spirit::x3::rule<model::classical_field_id, model::classical_field_id> rule{"classical field id"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<int> {
	template<class Context>
	auto operator()(Context) const {
		boost::spirit::x3::rule<int, int> rule{"int"};
		return (rule = boost::spirit::x3::int_);
	}
};

template<>
struct rule_for_impl<index_core> {
	auto operator()(lagrangian_parsing_context lp_context) const {
		auto index_uuid_gen = lp_context.index_id_lookup_and_generate;
		auto rule_def = (rule_for<int>(lp_context) | rule_for<string_id>(index_uuid_gen)).operator[](
				[](auto &&x3_context) {
					boost::spirit::x3::_val(x3_context) = support::to_std_variant<int, boost::uuids::uuid>(
							boost::spirit::x3::_attr(x3_context));
				});
		
		boost::spirit::x3::rule<index_core, std::variant<int, boost::uuids::uuid>> rule{"index core"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::spacetime_index> {
	template<class Context>
	auto operator()(Context context) const {
		const auto variance_def = boost::spirit::x3::lit("_").operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).variance = mathutils::spacetime_index::index_variance::lower;
		}) | boost::spirit::x3::lit("^").operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).variance = mathutils::spacetime_index::index_variance::upper;
		});
		
		auto index_core_def = rule_for<index_core>(context);
		
		const auto id_def = ((boost::spirit::x3::lit("{") >> index_core_def >> boost::spirit::x3::lit("}")) |
							 index_core_def);
		
		boost::spirit::x3::rule<mathutils::spacetime_index, mathutils::spacetime_index> rule{"spacetime index"};
		return (rule = (variance_def >> id_def));
	}
};

template<>
struct rule_for_impl<mathutils::gamma_matrix> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = boost::spirit::x3::lit("\\gamma") >> rule_for<mathutils::spacetime_index>(context);
		boost::spirit::x3::rule<mathutils::gamma_matrix, mathutils::gamma_matrix> rule{"gamma matrix"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::sigma_matrix> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = boost::spirit::x3::lit("\\sigma_{") >> rule_for<mathutils::spacetime_index>(context) >> ","
															>> rule_for<mathutils::spacetime_index>(context);
		boost::spirit::x3::rule<mathutils::sigma_matrix, mathutils::sigma_matrix> rule{"sigma_matrix"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::spacetime_derivative> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = boost::spirit::x3::lit("\\partial_") >> rule_for<mathutils::spacetime_index>(context);
		boost::spirit::x3::rule<mathutils::spacetime_derivative, mathutils::spacetime_derivative> rule{
				"spacetime derivative"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::dirac_operator> {
	template<class Context>
	auto operator()(Context) const {
		auto rule_def = boost::spirit::x3::lit("\\DiracOperator").operator[]([](auto &&) {});
		boost::spirit::x3::rule<mathutils::dirac_operator, mathutils::dirac_operator> rule{"dirac operator"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<model::lagrangian_symbol> {
	template<class Context>
	auto operator()(Context context) const {
		using value_type = std::decay_t<decltype(std::declval<model::lagrangian_symbol>().value)>;
		boost::spirit::x3::rule<model::lagrangian_symbol, model::lagrangian_symbol> rule{"lagrangian symbol"};
		
		return (rule = rule_for<value_type>(context));
	}
};

}
*/

#endif //PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_HPP
