//
// Created by jayz on 09.01.20.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_ARITHMETIC_HPP
#define PQUANTUM_PARSING_PARSER_RULES_ARITHMETIC_HPP

#include "model/lagrangian.hpp"
#include "lazy.hpp"

#include <numeric>

namespace PQuantum::parsing::parser_rules
{
template<>
struct node_rule_for_impl<>
{
	template<class Context>
	auto operator()( Context context ) const
	{
		auto product_rule = rule_for < arithmetic_product_tag < ArithmeticType, supports_division>>( context );
		auto plus_minus_rule = rule_for<plus_minus_operator_tag>( context );
		
		auto rule_def = (( -plus_minus_rule ) >> *( product_rule >> plus_minus_rule ) >> product_rule ).operator[](
		[]( auto &&x3_context ) {
			auto &&attr = boost::spirit::x3::_attr( x3_context );
			auto &&have_first_op = boost::fusion::at_c<0>( attr );
			auto &&first_terms_and_ops = boost::fusion::at_c<1>( attr );
			auto &&last_term = boost::fusion::at_c<2>( attr );
			
			auto first_terms = ( first_terms_and_ops | boost::adaptors::transformed( detail::wrap_at_c<0>{} ));
			auto last_ops = ( first_terms_and_ops | boost::adaptors::transformed( detail::wrap_at_c<1>{} ));
			
			auto all_terms = boost::join( first_terms, boost::make_iterator_range( &last_term, ( &last_term ) + 1 ));
			
			auto first_term = *std::begin( all_terms );
			if( have_first_op && ( *have_first_op == '-' ))
				first_term = -first_term;
			auto last_terms = boost::make_iterator_range( ++std::begin( all_terms ), std::end( all_terms ));
			auto last_terms_and_ops = boost::combine( last_terms, last_ops );
			
			auto evaluate = []( auto &x, auto &&y_and_op ) {
				auto op = boost::fusion::at_c<1>( y_and_op );
				if( op == '+' )
					return x += boost::fusion::at_c<0>( std::forward<decltype( y_and_op )>( y_and_op ));
				else
					return x -= boost::fusion::at_c<0>( std::forward<decltype( y_and_op )>( y_and_op ));
			};
			
			std::accumulate( std::begin( last_terms_and_ops ), std::end( last_terms_and_ops ), first_term, evaluate );
			boost::spirit::x3::_val( x3_context ) = std::move( first_term );
		} );
		
		boost::spirit::x3::rule<arithmetic_tag < ArithmeticType>, ArithmeticType > rule{ "arithmetic" };
		return ( rule = rule_def );
	}
};


namespace detail
{
template<int N>
struct wrap_at_c
{
	template<class T>
	decltype( auto ) operator()( T &&t ) const
	{
		return boost::fusion::at_c<N>( std::forward<T>( t ));
	}
};
}






template<class ArithmeticType, bool supports_division = true> struct arithmetic_tag;
template<class ArithmeticType, bool supports_division = true> struct arithmetic_core_tag;
template<class ArithmeticType, bool supports_division = true> struct arithmetic_product_tag;

template<class ArithmeticType> struct no_arithmetic_tag;

struct plus_minus_operator_tag;

template<bool supports_division = true>
struct mul_div_operator_tag;

template<> struct rule_for_impl<plus_minus_operator_tag> {
	template<class Context> auto operator()( Context ) const {
		using boost::spirit::x3::char_;
		auto rule_def = char_('+') | char_('-');
		boost::spirit::x3::rule<plus_minus_operator_tag, char> rule{"plus/minus"};
		return (rule = rule_def);
	}
};

template<> struct rule_for_impl<mul_div_operator_tag<true>> {
	template<class Context> auto operator()( Context ) const {
		using boost::spirit::x3::char_;
		auto rule_def = char_('*') | char_('/');
		boost::spirit::x3::rule<mul_div_operator_tag<true>, char> rule{"mul/div"};
		return (rule = rule_def);
	}
};

template<> struct rule_for_impl<mul_div_operator_tag<false>> {
	template<class Context> auto operator()( Context ) const {
		using boost::spirit::x3::char_;
		auto rule_def = char_('*');
		boost::spirit::x3::rule<mul_div_operator_tag<false>, char> rule{"mul"};
		return (rule = rule_def);
	}
};

template<class ArithmeticType, bool supports_division>
struct rule_for_impl<arithmetic_core_tag<ArithmeticType, supports_division>> {
	template<class Context> auto operator()( Context context ) const {
		using boost::spirit::x3::lit;
		
		auto brackets_rule = lit("(") >> make_lazy<arithmetic_tag<ArithmeticType, supports_division>, ArithmeticType>() >> lit(")");
		auto no_brackets_rule = rule_for<no_arithmetic_tag<ArithmeticType>>( context );
		
		auto rule_def = (brackets_rule | no_brackets_rule);
		boost::spirit::x3::rule<arithmetic_core_tag<ArithmeticType, supports_division>, ArithmeticType> rule{"arithmetic core"};
		return (rule = rule_def);
	}
};

template<class ArithmeticType, bool supports_division>
struct rule_for_impl<arithmetic_product_tag<ArithmeticType, supports_division>> {
	template<class Context>
	auto operator()( Context context ) const {
		auto core_rule = rule_for<arithmetic_core_tag<ArithmeticType, supports_division>>( context );
		auto muldiv_rule = rule_for<mul_div_operator_tag<supports_division>>( context );
		
		auto rule_def = (*(core_rule >> muldiv_rule) >> core_rule).operator[](
			[] (auto &&x3_context) {
				auto &&attr = boost::spirit::x3::_attr(x3_context);
				auto &&first_terms_and_ops = boost::fusion::at_c<0>(attr);
				auto &&last_term = boost::fusion::at_c<1>(attr);
				
				auto first_terms = (first_terms_and_ops | boost::adaptors::transformed( detail::wrap_at_c<0>{} ));
				
				auto all_terms = boost::join(
					first_terms,
					boost::make_iterator_range( &last_term, (&last_term) + 1 )
				);
				
				if constexpr( supports_division ) {
					auto ops = (first_terms_and_ops | boost::adaptors::transformed( detail::wrap_at_c<1>{} ));
					
					auto evaluate = [] ( auto &x, auto &&y_and_op ) {
						auto op = boost::fusion::at_c<1>( y_and_op );
						if( op == '*' )
							return x *= boost::fusion::at_c<0>( std::forward<decltype(y_and_op)>( y_and_op ) );
						else
							return x /= boost::fusion::at_c<0>( std::forward<decltype(y_and_op)>( y_and_op ) );
					};
					
					auto first_term = *std::begin( all_terms );
					auto last_terms = boost::make_iterator_range( ++std::begin( all_terms ), std::end( all_terms ) );
					auto last_terms_and_ops = boost::combine( last_terms, ops );
					
					std::accumulate( std::begin( last_terms_and_ops ), std::end( last_terms_and_ops ),
												   first_term, evaluate );
					boost::spirit::x3::_val(x3_context) = std::move(first_term);
				} else {
					auto evaluate = [] ( auto &x, auto &&y ) {
						return x *= std::forward<decltype(y)>( y );
					};
					
					auto result = std::accumulate( ++std::begin( all_terms ), std::end( all_terms ),
												   *std::begin( all_terms ), evaluate );
					boost::spirit::x3::_val(x3_context) = std::move(result);
				}
			}
		);
		
		boost::spirit::x3::rule<arithmetic_product_tag<ArithmeticType, supports_division>, ArithmeticType> rule{"arithmetic product"};
		return (rule = rule_def);
	}
};


}

#endif //PQUANTUM_PARSING_PARSER_RULES_ARITHMETIC_HPP
