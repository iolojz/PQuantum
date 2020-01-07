//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_HPP
#define PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_HPP

#include "../parse.hpp"
#include "string_id.hpp"
#include "std_variant.hpp"
#include "free_r_algebra.hpp"
#include "polynomial_expression.hpp"

#include "support/variant.hpp"

namespace PQuantum::parsing::parser_rules {
struct lagrangian_parsing_context {
	std::function<boost::uuids::uuid(std::string)> field_id_lookup;
	std::function<boost::uuids::uuid(std::string)> parameter_id_lookup;
	std::function<boost::uuids::uuid(std::string)> index_id_lookup_and_generate;
};

struct index_core;

template<>
struct rule_for_impl<model::classical_field_id> {
	template<class LPContext>
	auto operator()(LPContext lp_context) const {
		auto rule_def = rule_for<string_id>(lp_context.field_id_lookup);
		return (boost::spirit::x3::rule<model::classical_field_id, model::classical_field_id>{} = rule_def);
	}
};

template<>
struct rule_for_impl<int> {
	template<class Context>
	auto operator()(Context &&) const {
		return (boost::spirit::x3::rule<int, int>{} = boost::spirit::x3::int_);
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
		return (boost::spirit::x3::rule<index_core, std::variant<int, boost::uuids::uuid>>{} = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::spacetime_index> {
	template<class LPContext>
	auto operator()(LPContext lp_context) const {
		const auto variance_def = boost::spirit::x3::lit("_").operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).variance = mathutils::spacetime_index::index_variance::lower;
		}) | boost::spirit::x3::lit("^").operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).variance = mathutils::spacetime_index::index_variance::upper;
		});
		
		auto index_core_def = rule_for<index_core>(lp_context);
		
		const auto id_def = ((boost::spirit::x3::lit("{") >> index_core_def >> boost::spirit::x3::lit("}")) |
							 index_core_def);
		return (boost::spirit::x3::rule<mathutils::spacetime_index, mathutils::spacetime_index>{} = (variance_def
				>> id_def));
	}
};

template<>
struct rule_for_impl<mathutils::gamma_matrix> {
	template<class LPContext>
	auto operator()(LPContext lp_context) const {
		auto rule_def = boost::spirit::x3::lit("\\gamma") >> rule_for<mathutils::spacetime_index>(lp_context);
		return (boost::spirit::x3::rule<mathutils::gamma_matrix, mathutils::gamma_matrix>{} = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::sigma_matrix> {
	template<class LPContext>
	auto operator()(LPContext lp_context) const {
		auto rule_def = boost::spirit::x3::lit("\\sigma_{") >> rule_for<mathutils::spacetime_index>(lp_context) >> ","
															>> rule_for<mathutils::spacetime_index>(lp_context);
		return (boost::spirit::x3::rule<mathutils::sigma_matrix, mathutils::sigma_matrix>{} = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::spacetime_derivative> {
	template<class LPContext>
	auto operator()(LPContext lp_context) const {
		auto rule_def = boost::spirit::x3::lit("\\partial_") >> rule_for<mathutils::spacetime_index>(lp_context);
		return (boost::spirit::x3::rule<mathutils::spacetime_derivative, mathutils::spacetime_derivative>{} = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::dirac_operator> {
	template<class LPContext>
	auto operator()(LPContext) const {
		auto rule_def = boost::spirit::x3::lit("\\DiracOperator").operator[]([](auto &&) {});
		return (boost::spirit::x3::rule<mathutils::dirac_operator, mathutils::dirac_operator>{} = rule_def);
	}
};


template<>
struct rule_for_impl<model::lagrangian_symbol> {
	auto operator()(lagrangian_parsing_context lp_context) const {
		using value_type = std::decay_t<decltype(std::declval<model::lagrangian_symbol>().value)>;
		boost::spirit::x3::rule<model::lagrangian_symbol, model::lagrangian_symbol> rule;
		
		return (rule = rule_for<value_type>(lp_context));
	}
};

}

#include "polynomial_expression.hpp"
#include "free_r_algebra.hpp"

#endif //PQUANTUM_PARSING_PARSER_RULES_LAGRANGIAN_HPP
