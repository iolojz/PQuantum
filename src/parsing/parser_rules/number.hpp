//
// Created by jayz on 08.01.20.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_NUMBER_HPP
#define PQUANTUM_PARSING_PARSER_RULES_NUMBER_HPP

namespace PQuantum::parsing::parser_rules {

template<class FRA>
struct rule_for_impl<number_core_tag> {
	template<class Context>
	auto operator()(Context context) const {
		auto rule_def = (fra_core_with_coefficient | fra_core_without_coefficient);
		
		boost::spirit::x3::rule<number_core_tag, mathutils::number> rule{"number_core"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<number_factor_tag> {
	template<class Context>
	auto operator()(Context context) const {
		auto core_rule = rule_for<number_core_tag>(context);
		
		auto rule_def = ((boost::spirit::x3::lit("(") >> core_rule >> boost::spirit::x3::lit(")")) | core_rule);
		
		boost::spirit::x3::rule<number_factor_tag, mathutils::number> rule{"number_factor"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<number_product_tag> {
	template<class Context>
	auto operator()(Context context) const {
		auto factor_rule = rule_for<number_factor_tag>(context);
		auto muldiv_rule = rule_for<number_muldiv_tag>(context);
		
		auto rule_def = (*(factor_rule >> muldiv_rule) >> factor_rule).operator[](
				[](auto &&x3_context) {
					const auto &attr = boost::spirit::x3::_attr(x3_context);
					
					auto &factors_and_ops = boost::fusion::at_c<0>(attr);
					auto &last_factor = boost::fusion::at_c<1>(attr);
					// TODO...
				});
		
		boost::spirit::x3::rule<number_product_tag, mathutils::number> rule{"number_product"};
		return (rule = rule_def);
	}
};

template<class FRA>
struct rule_for_impl<number_sign_tag> {
	template<class Context>
	auto operator()(Context) const {
		auto rule_def = (boost::spirit::x3::char_('+') | boost::spirit::x3::char_('-'));
		boost::spirit::x3::rule<number_sign_tag, char> rule{"number_sign"};
		return (rule = rule_def);
	}
};

template<>
struct rule_for_impl<mathutils::number> {
	template<class Context>
	auto operator()(Context context) const {
		auto product_rule = rule_for<number_product_tag>(context);
		auto sign_rule = rule_for<number_sign_tag>(context);
		
		auto rule_def = ((-sign_rule) >> *(product_rule >> sign_rule) >> product_rule).operator[](
				[](auto &&x3_context) {
					const auto &attr = boost::spirit::x3::_attr(x3_context);
					
					const auto &preceeding_minus_sign = boost::fusion::at_c<0>(attr);
					auto &summands_and_ops = boost::fusion::at_c<1>(attr);
					auto &last_summand = boost::fusion::at_c<2>(attr);
					
					char first_sign = bool(preceeding_minus_sign) ? '-' : '+';
					
					std::vector<char> ops{ first_sign };
					std::transform( std::begin( summands_and_ops ), std::end( summands_and_ops ),
									std::back_inserter( ops ), []( auto &&number_and_op ) {
								return std::move( boost::fusion::at_c<1>( number_and_op ) );
							} );
					
					std::vector<mathutils::number> summands;
					std::transform( std::begin( summands_and_ops ), std::end( summands_and_ops ),
									std::back_inserter( summands ), []( auto &&number_and_op ) {
								return std::move( boost::fusion::at_c<0>( number_and_op ) );
							} );
					summands.push_back( std::move( last_summand ) );
					
					auto add_up = [] ( FRA &fra, const auto &op_and_number ) {
						if( boost::fusion::at_c<0>( op_and_number ) == '-' )
							return fra_ring::subtract_assign( fra, boost::fusion::at_c<1>( op_and_number ) );
						else
							return fra_ring::add_assign( fra, boost::fusion::at_c<1>( op_and_number ) );
					};
					
					auto sum = boost::accumulate( boost::range::combine( ops, summands ), fra_ring::zero(), add_up );
					boost::spirit::x3::_val(x3_context) = std::move(sum);
				});
		
		boost::spirit::x3::rule<mathutils::number, mathutils::number> rule{"number_sum"};
		return (rule = rule_def);
	}
};
};
}

#endif //PQUANTUM_PARSING_PARSER_RULES_NUMBER_HPP
