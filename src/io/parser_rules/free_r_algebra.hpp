//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_FREE_R_ALGEBRA_HPP
#define PQUANTUM_IO_FREE_R_ALGEBRA_HPP

#include "rule_for_symbol_fwd.hpp"
#include "std_vector.hpp"
#include "../qft_json_parser.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/copy.hpp>

namespace PQuantum::io::parser_rules {
struct free_r_algebra_tag;

template<class FRA>
struct fra_coefficient_tag;

template<class FRA>
struct fra_core_with_coefficient_tag;

template<class FRA>
struct fra_core_without_coefficient_tag;

template<class FRA>
struct fra_sum_rule_tag;

template<class FRA>
struct fra_product_rule_tag;

template<class FRA>
struct rule_for_symbol_impl<fra_coefficient_tag<FRA>> {
	template<class Context>
	auto operator()(Context &&context) const {
		using coefficient = typename FRA::coefficient;
		auto coefficient_rule = rule_for_symbol<coefficient>(std::forward<Context>(context));
		
		auto rule_def = (+coefficient_rule).operator[]([](auto &&x3_context) {
			//const auto &coefficients = boost::spirit::x3::_attr( x3_context );
			
			// FIXME: use commented out code below
			/*auto c = boost::fusion::fold( coefficients, [] ( auto &&c1, auto &&c2 ) {
				using C1 = decltype(c1);
				using C2 = decltype(c2);
				
				return FRA::coefficient_ring::multiply( std::forward<C1>( c1 ), std::forward<C2>( c2 ) );
			} );*/
			auto c = FRA::coefficient_ring::one();
			
			boost::spirit::x3::_val(x3_context) = std::move(c);
		});
		
		return (boost::spirit::x3::rule<fra_coefficient_tag<FRA>, coefficient>{} = rule_def);
	}
};

template<class FRA>
struct rule_for_symbol_impl<fra_core_with_coefficient_tag<FRA>> {
	template<class Context>
	auto operator()(Context &&context) const {
		auto coefficient_rule = rule_for_symbol<fra_coefficient_tag<FRA>>(std::forward<Context>(context));
		auto symbol_rule = rule_for_symbol<typename FRA::symbol>(context);
		
		auto rule_def = (coefficient_rule >> *symbol_rule).operator[]([](auto &&x3_context) {
			const auto &attr = boost::spirit::x3::_attr(x3_context);
			const auto &c = boost::fusion::at_c<0>(attr);
			const auto &symbols = boost::fusion::at_c<1>(attr);
			
			boost::spirit::x3::_val(x3_context) = cxxmath::make<cxxmath::tag_of_t<FRA>>(c, symbols);
		});
		
		return (boost::spirit::x3::rule<fra_core_with_coefficient_tag<FRA>, FRA>{} = rule_def);
	}
};

template<class FRA>
struct rule_for_symbol_impl<fra_core_without_coefficient_tag<FRA>> {
	template<class Context>
	auto operator()(Context &&context) const {
		auto symbol_rule = rule_for_symbol<typename FRA::symbol>(std::forward<Context>(context));
		
		auto rule_def = (+symbol_rule).operator[]([](auto &&x3_context) {
			const auto &symbols = boost::spirit::x3::_attr(x3_context);
			
			boost::spirit::x3::_val(x3_context) = cxxmath::make<cxxmath::tag_of_t<FRA>>(FRA::coefficient_ring::one(),
																						symbols);
		});
		
		return (boost::spirit::x3::rule<fra_core_without_coefficient_tag<FRA>, FRA>{} = rule_def);
	}
};

template<class FRA>
struct rule_for_symbol_impl<FRA, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>> {
	template<class Context>
	auto operator()(Context &&context) const {
		auto core_rule = (rule_for_symbol<fra_core_with_coefficient_tag<FRA>>(context) |
						  rule_for_symbol<fra_core_without_coefficient_tag<FRA>>(context));
		
		// FIXME: use recursive rule
		//auto recursive_rule = boost::spirit::x3::rule<free_r_algebra_tag, FRA>{};
		
		auto factor_rule = (boost::spirit::x3::lit("(") >> core_rule >> boost::spirit::x3::lit(")"));
		auto product_rule_def = (*(factor_rule >> boost::spirit::x3::lit("*")) >> factor_rule).operator[](
				[](auto &&x3_context) {
					auto product = FRA{};
					// FIXME: ...
					/*auto product = boost::fusion::fold( boost::spirit::x3::_attr( x3_context ), FRA{}, []( auto &&fra1, auto &&fra2 ) {
						using FRA1 = decltype(fra1);
						using FRA2 = decltype(fra2);
						
						return cxxmath::multiply( std::forward<FRA1>( fra1 ), std::forward<FRA2>( fra2 ) );
					} );*/
					
					boost::spirit::x3::_val(x3_context) = std::move(product);
				});
		auto product_rule = (boost::spirit::x3::rule<fra_product_rule_tag<FRA>, FRA>{} = product_rule_def);
		
		auto sum_rule_def = (+product_rule).operator[]([](auto &&x3_context) {
			auto sum = boost::fusion::at_c<0>(boost::spirit::x3::_attr(x3_context));
			
			//sum = boost::fusion::at_c<0>( boost::spirit::x3::_attr(x3_context) );
			
			/*std::vector<FRA> foo;
			boost::fusion::copy( boost::spirit::x3::_attr(x3_context), foo );*/
			
			// FIXME: ...
			/*auto sum = boost::fusion::fold( boost::spirit::x3::_attr(x3_context), [] (auto &&fra1, auto &&fra2 ) {
				using FRA1 = decltype(fra1);
				using FRA2 = decltype(fra2);
				
				return cxxmath::add(std::forward<FRA1>(fra1), std::forward<FRA2>(fra2));
			} );*/
			
			boost::spirit::x3::_val(x3_context) = sum;
		});
		auto sum_rule = (boost::spirit::x3::rule<fra_sum_rule_tag<FRA>, FRA>{} = sum_rule_def);
		
		return (boost::spirit::x3::rule<free_r_algebra_tag, FRA>{} = sum_rule);
	}
};
}

#endif //PQUANTUM_IO_FREE_R_ALGEBRA_HPP
