//
// Created by jayz on 06.01.20.
//

#ifndef PQUANTUM_IO_FREE_R_ALGEBRA_HPP
#define PQUANTUM_IO_FREE_R_ALGEBRA_HPP

#include "rule_for_symbol_fwd.hpp"
#include "std_vector.hpp"
#include "model/qft_json_parser.hpp"

#include <numeric>
#include <boost/spirit/home/x3.hpp>

namespace PQuantum::io::parser_rules {
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
struct fra_sum_tag;

template<class FRA>
struct fra_product_tag;

template<class FRA>
struct rule_for_symbol_impl<FRA, std::enable_if_t<cxxmath::is_free_r_algebra_tag_v<cxxmath::tag_of_t<FRA>>>>
{
	template<class Context>
	auto operator()(Context &&context) const
	{
		using coefficient = typename FRA::coefficient;
		using coefficient_ring = typename FRA::coefficient_ring;
		using fra_ring = typename cxxmath::model_free_r_algebra::free_r_algebra_concepts<cxxmath::tag_of_t<FRA>>::ring;
		
		auto coefficient_rule_def = ( +rule_for_symbol<coefficient>( context )).operator[]( []( auto &&x3_context ) {
			const auto &coefficients = boost::spirit::x3::_attr( x3_context );
			auto c = std::accumulate( std::begin( coefficients ), std::end( coefficients ), coefficient_ring::one(),
									  coefficient_ring::multiply_assign );
			boost::spirit::x3::_val( x3_context ) = std::move( c );
		} );
		auto coefficient_rule = ( boost::spirit::x3::rule<fra_coefficient_tag<FRA>, coefficient>{} = coefficient_rule_def );
		auto symbol_rule = rule_for_symbol<typename FRA::symbol>( context );
		
		auto fra_core_with_coefficient_rule_def = ( coefficient_rule >> *symbol_rule ).operator[](
		[]( auto &&x3_context ) {
			const auto &attr = boost::spirit::x3::_attr( x3_context );
			const auto &c = boost::fusion::at_c<0>( attr );
			const auto &symbols = boost::fusion::at_c<1>( attr );
			
			boost::spirit::x3::_val( x3_context ) = cxxmath::make<cxxmath::tag_of_t<FRA>>( c, symbols );
		} );
		auto fra_core_with_coefficient = ( boost::spirit::x3::rule<fra_core_with_coefficient_tag<FRA>, FRA>{} = fra_core_with_coefficient_rule_def );
		
		auto fra_core_without_coefficient_rule_def = ( +symbol_rule ).operator[]( []( auto &&x3_context ) {
			const auto &symbols = boost::spirit::x3::_attr( x3_context );
			boost::spirit::x3::_val( x3_context ) = cxxmath::make<cxxmath::tag_of_t<FRA>>( FRA::coefficient_ring::one(),
																						   symbols );
		} );
		auto fra_core_without_coefficient = ( boost::spirit::x3::rule<fra_core_without_coefficient_tag<FRA>, FRA>{} = fra_core_without_coefficient_rule_def );
		
		auto core_rule_def = ( fra_core_with_coefficient | fra_core_without_coefficient );
		auto core_rule = ( boost::spirit::x3::rule<fra_core_tag<FRA>, FRA>{} = core_rule_def );
		
		// FIXME: use recursive rule
		//auto recursive_rule = boost::spirit::x3::rule<free_r_algebra_tag, FRA>{};
		
		auto factor_rule = ( boost::spirit::x3::lit( "(" ) >> core_rule >> boost::spirit::x3::lit( ")" ));
		auto product_rule_def = ( *( factor_rule >> boost::spirit::x3::lit( "*" )) >> factor_rule ).operator[](
		[]( auto &&x3_context ) {
			const auto &attr = boost::spirit::x3::_attr( x3_context );
			auto last_factor = boost::fusion::at_c<1>( attr );
			const auto &factors = boost::fusion::at_c<0>( attr );
			
			auto multiply_reverse = []( auto &fra1, auto &&fra2 ) {
				return fra_ring::multiply( std::forward<decltype( fra2 )>( fra2 ), fra1 );
			};
			auto product = std::accumulate( std::rbegin( factors ), std::rend( factors ), last_factor,
											multiply_reverse );
			boost::spirit::x3::_val( x3_context ) = std::move( last_factor );
		} );
		auto product_rule = ( boost::spirit::x3::rule<fra_product_tag<FRA>, FRA>{} = product_rule_def );
		
		auto sum_rule_def = ( +product_rule ).operator[]( []( auto &&x3_context ) {
			const auto &summands = boost::spirit::x3::_attr( x3_context );
			auto sum = std::accumulate( std::begin( summands ), std::end( summands ), fra_ring::zero(),
										fra_ring::add_assign );
			boost::spirit::x3::_val( x3_context ) = std::move( sum );
		} );
		auto sum_rule = ( boost::spirit::x3::rule<fra_sum_tag<FRA>, FRA>{} = sum_rule_def );
		
		return ( boost::spirit::x3::rule<free_r_algebra_tag, FRA>{} = sum_rule );
	}
};
}

#endif //PQUANTUM_IO_FREE_R_ALGEBRA_HPP
