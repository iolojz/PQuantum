//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_IO_LAGRANGIAN_HPP
#define PQUANTUM_IO_LAGRANGIAN_HPP

#include "string_id.hpp"
#include "free_r_algebra.hpp"
#include "polynomial_expression.hpp"
#include "model/qft_json_parser.hpp"
#include "../../support/variant.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::io::parser_rules {
struct lagrangian_symbol_tag;

template<>
struct rule_for_symbol_impl<model::classical_field_id> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		auto rule_def = rule_for_symbol<string_id>(lp_context.field_id_lookup);
		return (boost::spirit::x3::rule<model::classical_field_id, model::classical_field_id>{} = rule_def);
	}
};

struct int_tag;
struct index_core;

template<>
struct rule_for_symbol_impl<int> {
	template<class Context>
	auto operator()(Context &&) const {
		return (boost::spirit::x3::rule<int_tag, int>{} = boost::spirit::x3::int_);
	}
};

template<>
struct rule_for_symbol_impl<index_core> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		auto uuid_gen = lp_context.index_id_lookup_and_generate;
		
		auto rule_def = (rule_for_symbol<int>(uuid_gen) | rule_for_symbol<string_id>(uuid_gen)).operator[](
				[](auto &&x3_context) {
					boost::spirit::x3::_val(x3_context) = template_helpers::to_std_variant<int, boost::uuids::uuid>(
							boost::spirit::x3::_attr(x3_context));
				});
		return (boost::spirit::x3::rule<index_core, std::variant<int, boost::uuids::uuid>>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<mathutils::spacetime_index> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		const auto variance_def = boost::spirit::x3::lit("_").operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).variance = mathutils::spacetime_index::index_variance::lower;
		}) | boost::spirit::x3::lit("^").operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).variance = mathutils::spacetime_index::index_variance::upper;
		});
		const auto id_def = ((boost::spirit::x3::lit("{") >> rule_for_symbol<index_core>(lp_context)
														  >> boost::spirit::x3::lit("}")) |
							 rule_for_symbol<index_core>(lp_context));
		return (boost::spirit::x3::rule<mathutils::spacetime_index, mathutils::spacetime_index>{} = (variance_def
				>> id_def));
	}
};

template<>
struct rule_for_symbol_impl<mathutils::gamma_matrix> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		auto rule_def = boost::spirit::x3::lit("\\gamma") >> rule_for_symbol<mathutils::spacetime_index>(lp_context);
		return (boost::spirit::x3::rule<mathutils::gamma_matrix, mathutils::gamma_matrix>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<mathutils::sigma_matrix> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		auto rule_def =
				boost::spirit::x3::lit("\\sigma_{") >> rule_for_symbol<mathutils::spacetime_index>(lp_context) >> ","
													>> rule_for_symbol<mathutils::spacetime_index>(lp_context);
		return (boost::spirit::x3::rule<mathutils::sigma_matrix, mathutils::sigma_matrix>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<mathutils::spacetime_derivative> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		auto rule_def = boost::spirit::x3::lit("\\partial_") >> rule_for_symbol<mathutils::spacetime_index>(lp_context);
		return (boost::spirit::x3::rule<mathutils::spacetime_derivative, mathutils::spacetime_derivative>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<mathutils::dirac_operator> {
	template<class LPContext> auto operator()(LPContext) const {
		auto rule_def = boost::spirit::x3::lit("\\DiracOperator").operator[]([](auto &&) {});
		return (boost::spirit::x3::rule<mathutils::dirac_operator, mathutils::dirac_operator>{} = rule_def);
	}
};

template<>
struct rule_for_symbol_impl<model::lagrangian_symbol> {
	template<class LPContext> auto operator()(LPContext lp_context) const {
		using value_type = std::decay_t<decltype(std::declval<model::lagrangian_symbol>().value)>;
		
		auto rule_def = rule_for_symbol<value_type>(lp_context).operator[]([](auto &&x3_context) {
			boost::spirit::x3::_val(x3_context).value = boost::spirit::x3::_attr(x3_context);
		});
		return (boost::spirit::x3::rule<lagrangian_symbol_tag, model::lagrangian_symbol>{} = rule_def);
	}
};
}

#endif //PQUANTUM_IO_LAGRANGIAN_HPP
