//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_FREE_R_ALGEBRA_HPP
#define PQUANTUM_IO_FREE_R_ALGEBRA_HPP

#include "arithmetic.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum::parsing::parser_rules {
template<class FRA> struct fra_with_coefficient_tag;
template<class FRA> struct fra_without_coefficient_tag;

template<class FRA>
struct rule_for_impl<fra_with_coefficient_tag<FRA>, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>> {
	template<class Context>
	auto operator()(Context context) const {
		auto coefficient_rule = rule_for<typename FRA::coefficient>( context );
		auto symbol_rule = rule_for<typename FRA::symbol>( context );
		
		auto rule_def = ((+coefficient_rule) >> *symbol_rule).operator[]( [] ( auto &&x3_context ) {
			auto &&attr = boost::spirit::x3::_attr(x3_context);
			auto &&coefficients = boost::fusion::at_c<0>(attr);
			auto &&symbols = boost::fusion::at_c<1>(attr);
			
			auto coefficient = *std::begin( coefficients );
			std::accumulate( ++std::begin( coefficients ), std::end( coefficients ),
					coefficient, FRA::coefficient_ring::multiply_assign );
			
			boost::spirit::x3::_val(x3_context) = cxxmath::make<cxxmath::tag_of_t<FRA>>( std::move( coefficient ),
					std::begin( symbols ), std::end( symbols ) );
		} );
		boost::spirit::x3::rule<fra_with_coefficient_tag<FRA>, FRA> rule{"fra with coefficient"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<fra_without_coefficient_tag<FRA>, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>> {
	template<class Context>
	auto operator()(Context context) const {
		auto symbol_rule = rule_for<typename FRA::symbol>( context );
		
		auto rule_def = (+symbol_rule).operator[]( [] ( auto &&x3_context ) {
			auto &&symbols = boost::spirit::x3::_attr( x3_context );
			boost::spirit::x3::_val(x3_context) = cxxmath::make<cxxmath::tag_of_t<FRA>>(
				FRA::coefficient_ring::one(), std::begin( symbols ), std::end( symbols )
			);
		} );
		boost::spirit::x3::rule<fra_without_coefficient_tag<FRA>, FRA> rule{"fra with coefficient"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<no_arithmetic_tag<FRA>, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>> {
	template<class Context>
	auto operator()(Context context) const {
		auto with_coefficient_rule = rule_for<fra_with_coefficient_tag<FRA>>( context );
		auto without_coefficient_rule = rule_for<fra_without_coefficient_tag<FRA>>( context );
		
		auto rule_def = (with_coefficient_rule | without_coefficient_rule);
		boost::spirit::x3::rule<no_arithmetic_tag<FRA>, FRA> rule{"free_r_algebra_core"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<FRA, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = rule_for<arithmetic_tag<FRA, false>>( context );
		boost::spirit::x3::rule<cxxmath::tag_of_t<FRA>, FRA> rule{"free_r_algebra"};
		return (rule = rule_def);
	}
};
}

#endif //PQUANTUM_IO_FREE_R_ALGEBRA_HPP
