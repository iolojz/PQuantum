//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_FREE_R_ALGEBRA_HPP
#define PQUANTUM_IO_FREE_R_ALGEBRA_HPP

#include "rule_for_fwd.hpp"

#include <numeric>
#include <boost/spirit/home/x3.hpp>

#include <boost/range/numeric.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm_ext/push_front.hpp>

namespace PQuantum::parsing::parser_rules {
template<class FRA>
struct free_r_algebra_tag;
template<class FRA>
struct fra_coefficient_tag;
template<class FRA>
struct fra_core_with_coefficient_tag;
template<class FRA>
struct fra_core_without_coefficient_tag;
template<class FRA>
struct fra_core_tag;
template<class FRA>
struct fra_factor_tag;
template<class FRA>
struct fra_product_tag;
template<class FRA>
struct fra_sign_tag;

template<class FRA>
struct rule_for_impl<fra_coefficient_tag<FRA>> {
	template<class Context>
	auto operator()(Context context) const {
		using coefficient = typename FRA::coefficient;
		using coefficient_ring = typename FRA::coefficient_ring;
		
		auto rule_def = (+rule_for<coefficient>(context)).operator[]([](auto &&x3_context) {
			const auto &coefficients = boost::spirit::x3::_attr(x3_context);
			auto c = std::accumulate(std::begin(coefficients), std::end(coefficients), coefficient_ring::one(),
									 coefficient_ring::multiply_assign);
			boost::spirit::x3::_val(x3_context) = std::move(c);
		});
		
		boost::spirit::x3::rule<fra_coefficient_tag<FRA>, coefficient> rule{"fra_coefficient"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_core_with_coefficient_tag<FRA>> {
	template<class Context>
	auto operator()(Context context) const {
		auto coefficient_rule = rule_for<fra_coefficient_tag<FRA>>(context);
		auto symbol_rule = rule_for<typename FRA::symbol>(context);
		
		auto rule_def = (coefficient_rule >> *symbol_rule).operator[]([](auto &&x3_context) {
			const auto &attr = boost::spirit::x3::_attr(x3_context);
			const auto &c = boost::fusion::at_c<0>(attr);
			const auto &symbols = boost::fusion::at_c<1>(attr);
			
			boost::spirit::x3::_val(x3_context) = cxxmath::make<cxxmath::tag_of_t<FRA>>(c, symbols);
		});
		
		boost::spirit::x3::rule<fra_core_with_coefficient_tag<FRA>, FRA> rule{"fra_core_with_coefficient"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_core_without_coefficient_tag<FRA>> {
	template<class Context>
	auto operator()(Context context) const {
		auto symbol_rule = rule_for<typename FRA::symbol>(context);
		
		auto rule_def = (+symbol_rule).operator[]([](auto &&x3_context) {
			const auto &symbols = boost::spirit::x3::_attr(x3_context);
			boost::spirit::x3::_val(x3_context) = cxxmath::make<cxxmath::tag_of_t<FRA>>(FRA::coefficient_ring::one(),
																						symbols);
		});
		
		boost::spirit::x3::rule<fra_core_without_coefficient_tag<FRA>, FRA> rule{"fra_core_without_coefficient"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_core_tag<FRA>> {
	template<class Context>
	auto operator()(Context context) const {
		auto fra_core_with_coefficient = rule_for<fra_core_with_coefficient_tag<FRA>>(context);
		auto fra_core_without_coefficient = rule_for<fra_core_without_coefficient_tag<FRA>>(context);
		
		auto rule_def = (fra_core_with_coefficient | fra_core_without_coefficient);
		
		boost::spirit::x3::rule<fra_core_tag<FRA>, FRA> rule{"fra_core"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_factor_tag<FRA>> {
	template<class Context>
	auto operator()(Context context) const {
		auto core_rule = rule_for<fra_core_tag<FRA>>(context);
		
		auto rule_def = ((boost::spirit::x3::lit("(") >> core_rule >> boost::spirit::x3::lit(")")) | core_rule);
		
		boost::spirit::x3::rule<fra_factor_tag<FRA>, FRA> rule{"fra_factor"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_product_tag<FRA>> {
	template<class Context>
	auto operator()(Context context) const {
		using fra_ring = typename cxxmath::model_free_r_algebra::free_r_algebra_concepts<cxxmath::tag_of_t<FRA>>::ring;
		auto factor_rule = rule_for<fra_factor_tag<FRA>>(context);
		
		auto rule_def = (*(factor_rule >> boost::spirit::x3::lit('*')) >> factor_rule).operator[](
				[](auto &&x3_context) {
					const auto &attr = boost::spirit::x3::_attr(x3_context);
					auto last_factor = boost::fusion::at_c<1>(attr);
					const auto &factors = boost::fusion::at_c<0>(attr);
					
					auto multiply_reverse = [](auto &fra1, auto &&fra2) {
						return fra_ring::multiply(std::forward<decltype(fra2)>(fra2), fra1);
					};
					auto product = std::accumulate(std::rbegin(factors), std::rend(factors), last_factor,
												   multiply_reverse);
					boost::spirit::x3::_val(x3_context) = std::move(product);
				});
		
		boost::spirit::x3::rule<fra_product_tag<FRA>, FRA> rule{"fra_product"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_sign_tag<FRA>> {
	template<class Context>
	auto operator()(Context) const {
		auto rule_def = (boost::spirit::x3::char_('+') | boost::spirit::x3::char_('-'));
		boost::spirit::x3::rule<fra_sign_tag<FRA>, char> rule{"fra_sign"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<FRA, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>> {
	template<class Context>
	auto operator()(Context context) const {
		using fra_ring = typename cxxmath::model_free_r_algebra::free_r_algebra_concepts<cxxmath::tag_of_t<FRA>>::ring;
		auto product_rule = rule_for<fra_product_tag<FRA>>(context);
		auto sign_rule = rule_for<fra_sign_tag<FRA>>(context);
		
		auto rule_def = ((-sign_rule) >> *(product_rule >> sign_rule) >> product_rule).operator[](
				[](auto &&x3_context) {
					const auto &attr = boost::spirit::x3::_attr(x3_context);
					
					const auto &preceeding_minus_sign = boost::fusion::at_c<0>(attr);
					auto &summands_and_ops = boost::fusion::at_c<1>(attr);
					auto &last_summand = boost::fusion::at_c<2>(attr);
					
					char first_sign = bool(preceeding_minus_sign) ? '-' : '+';
					
					std::vector<char> ops{ first_sign };
					std::transform( std::begin( summands_and_ops ), std::end( summands_and_ops ),
									std::back_inserter( ops ), []( auto &&fra_and_op ) {
								return std::move( boost::fusion::at_c<1>( fra_and_op ) );
							} );
					
					std::vector<FRA> summands;
					std::transform( std::begin( summands_and_ops ), std::end( summands_and_ops ),
									std::back_inserter( summands ), []( auto &&fra_and_op ) {
								return std::move( boost::fusion::at_c<0>( fra_and_op ) );
							} );
					summands.push_back( std::move( last_summand ) );
					
					auto add_up = [] ( FRA &fra, const auto &op_and_fra ) {
						if( boost::fusion::at_c<0>( op_and_fra ) == '-' )
							return fra_ring::subtract_assign( fra, boost::fusion::at_c<1>( op_and_fra ) );
						else
							return fra_ring::add_assign( fra, boost::fusion::at_c<1>( op_and_fra ) );
					};
					
					auto sum = boost::accumulate( boost::range::combine( ops, summands ), fra_ring::zero(), add_up );
					boost::spirit::x3::_val(x3_context) = std::move(sum);
				});
		
		boost::spirit::x3::rule<free_r_algebra_tag<FRA>, FRA> rule{"fra_sum"};
		return (rule = rule_def);
	}
};
}

#endif //PQUANTUM_IO_FREE_R_ALGEBRA_HPP
