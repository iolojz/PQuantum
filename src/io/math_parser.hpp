//
// Created by jayz on 16.08.19.
//

#ifndef PQUANTUM_MATH_PARSER_HPP
#define PQUANTUM_MATH_PARSER_HPP

#include "logging.hpp"
#include "error/control.hpp"

#include "mathutils/expressions.hpp"
#include "model/classical_field.hpp"

#include <variant>
#include <mathutils/linear_operators.hpp>
#include <boost/spirit/home/x3.hpp>

namespace PQuantum
{
namespace io
{
namespace detail
{
template<class Symbol>
struct rule_for_symbol;

/*
#ifdef PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE
#error "PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE macro already defined"
#endif

#define PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(parser) \
template <typename Iterator, typename Context> \
inline bool parse_rule( decltype(rule), \
	Iterator& first, Iterator const& last, \
	Context const& context, decltype(rule)::attribute_type& attr )\
{ return parser.parse(first, last, context, boost::spirit::x3::unused, attr ); }
*/

template<>
struct rule_for_symbol<int>
{
	auto operator()( void )
	{ return boost::spirit::x3::int_; }
};

template<>
class rule_for_symbol<mathutils::spacetime_index>
: boost::spirit::x3::rule<rule_for_symbol<mathutils::spacetime_index>, mathutils::spacetime_index>
{
	struct assign_variance_lower
	{
		template<class Context>
		void operator()( const Context &context )
		{
			boost::spirit::x3::_val( context ).variance = mathutils::spacetime_index::index_variance::lower;
		}
	};
	
	struct assign_variance_upper
	{
		template<class Context>
		void operator()( const Context &context )
		{
			boost::spirit::x3::_val( context ).variance = mathutils::spacetime_index::index_variance::upper;
		}
	};
	
	struct assign_id
	{
		template<class Context>
		void operator()( const Context &context )
		{
			boost::spirit::x3::_val( context ).id = boost::spirit::x3::_attr( context );
		}
	};

public:
	rule_for_symbol( void )
	: boost::spirit::x3::rule<rule_for_symbol<mathutils::spacetime_index>, mathutils::spacetime_index>(
	"parser_for_symbol<spacetime_index>" )
	{}
	
	auto operator()( void )
	{
		const auto variance_def =
		boost::spirit::x3::lit( "_" )[assign_variance_lower{}] | boost::spirit::x3::lit( "^" )[assign_variance_upper{}];
		const auto id_def = (( boost::spirit::x3::lit( "{" ) >> ( boost::spirit::x3::int_ | +boost::spirit::x3::char_ )
															 >> boost::spirit::x3::lit( "}" )) |
							 ( boost::spirit::x3::int_ | +boost::spirit::x3::char_ ))[assign_id{}];
		
		return variance_def >> id_def;
	}
};

template<>
struct rule_for_symbol<mathutils::gamma_matrix>
{
	auto operator()( void )
	{ return boost::spirit::x3::lit( "\\gamma" ) >> parser_for_symbol < mathutils::spacetime_index > {}(); }
};

template<>
struct rule_for_symbol<mathutils::sigma_matrix>
{
	auto operator()( void )
	{
		return boost::spirit::x3::lit( "\\sigma_{" ) >> parser_for_symbol < mathutils::spacetime_index >
			   {}() >> "," >> parser_for_symbol < mathutils::spacetime_index > {}();
	}
};

template<>
struct rule_for_symbol<mathutils::spacetime_derivative>
{
	auto operator()( void )
	{ return boost::spirit::x3::lit( "\\partial_" ) >> parser_for_symbol < mathutils::spacetime_index > {}(); }
};

template<>
struct rule_for_symbol<mathutils::dirac_operator>
{
	auto operator()( void )
	{ return boost::spirit::x3::lit( "\\DiracOperator" ); }
};

template<>
struct rule_for_symbol<model::classical_field_id>
{
	// TODO: add indices to field, turn attribute into classical_field_id
	auto operator()( void )
	{ return +boost::spirit::x3::char_; }
};

template<class ...Alternatives>
struct rule_for_symbol<std::variant<Alternatives...>>
{
	auto operator()( void )
	{ return (parser_for_symbol < Alternatives > {}() | ...); }
};
}

template<class Symbol>
auto rule_for_symbol( void )
{
	return detail::parser_for_symbol<Symbol>{}();
}

template<class Symbol>
std::vector<Symbol> parse_symbols( const std::string &string )
{
	BOOST_LOG_NAMED_SCOPE( "io::parse_symbols()" );
	io::severity_logger logger;
	
	std::vector<Symbol> symbols;
	const auto symbol_parser = parser_for_symbol<Symbol>();
	auto it = string.begin();
	
	bool parsing_result = boost::spirit::x3::phrase_parse( it, string.end(), *symbol_parser,
														   boost::spirit::x3::ascii::space /*,
 * 		symbols
 */
	);
	
	if( parsing_result == false ) {
		BOOST_LOG_SEV( logger, severity_level::error ) << "Cannot parse input string: \"" << string << "\"";
		control::exit_upon_error();
	}
	
	if( it != string.end()) {
		BOOST_LOG_SEV( logger, severity_level::error ) << "Cannot parse end of input string: \""
													   << std::string( it, string.end()) << "\"";
	}
	
	return symbols;
}

mathutils::polynomial_expression parse_polynomial_expression( const std::string & );
}
}

#endif //PQUANTUM_MATH_PARSER_HPP
