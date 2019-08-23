//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_LAGRANGIAN_SYMBOL_HPP
#define PQUANTUM_LAGRANGIAN_SYMBOL_HPP

#include "rule_for_symbol_fwd.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum {
namespace io {
namespace parser_rules
{
struct string_id
{
};

template<>
struct rule_for_symbol_impl<string_id>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		boost::spirit::x3::rule<string_id, boost::uuids::uuid> rule;
		rule = +boost::spirit::x3::char_[[&uuid_gen]( auto &&context ) {
			boost::spirit::x3::_val( context ) = uuid_gen( boost::spirit::x3::_attr( context ));
		}];
		return rule;
	}
};

template<>
struct rule_for_symbol<mathutils::spacetime_index>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		const auto index_core = boost::spirit::x3::int_ | string_id;
		
		const auto variance_def = boost::spirit::x3::lit( "_" )[[]( auto &&context ) {
			boost::spirit::x3::_val(context).variance = mathutils::spacetime_index::index_variance::lower;
		}] | boost::spirit::x3::lit( "^" )[]( auto && context )
		{
			boost::spirit::x3::_val(context).variance = mathutils::spacetime_index::index_variance::upper;
		}];
		const auto id_def = (( boost::spirit::x3::lit( "{" ) >> index_core
														  >> boost::spirit::x3::lit("}")) | index_core )[[&uuid_gen](
		auto &&context ) {
			boost::spirit::x3::_val( context ).id = boost::spirit::x3::_attr( context );
		}];
		
		boost::spirit::x3::rule<mathutils::spacetime_index, mathutils::spacetime_index> rule;
		rule = variance_def >> id_def;
		return rule;
	}
};

*/

template<>
struct rule_for_symbol_impl<mathutils::gamma_matrix>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		return boost::spirit::x3::lit( "\\gamma" ) >> rule_for_symbol < mathutils::spacetime_index >
			   ( std::forward<StringToUUID>( uuid_gen ));
	}
};

template<>
struct rule_for_symbol_impl<mathutils::sigma_matrix>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		return boost::spirit::x3::lit( "\\sigma_{" ) >> rule_for_symbol < mathutils::spacetime_index >
			   ( std::forward<StringToUUID>( uuid_gen )) >> "," >> rule_for_symbol < mathutils::spacetime_index >
			   ( std::forward<StringToUUID>( uuid_gen ));
	}
};

template<>
struct rule_for_symbol_impl<mathutils::spacetime_derivative>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		return boost::spirit::x3::lit( "\\partial_" ) >> rule_for_symbol < mathutils::spacetime_index >
			   ( std::forward<StringToUUID>( uuid_gen ));
	}
};

template<>
struct rule_for_symbol_impl<mathutils::dirac_operator>
{
	template<class Context>
	auto operator()( Context && ) const
	{
		return boost::spirit::x3::lit( "\\DiracOperator" );
	}
};

template<>
struct rule_for_symbol_impl<model::classical_field_id>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		boost::spirit::x3::rule<model::classical_field_id, model::classical_field_id> rule;
		rule %= rule_for_symbol < string_id > ( std::forward<StringToUUID>( uuid_gen ));
		return rule;
	}
};
}
}
}

#endif //PQUANTUM_LAGRANGIAN_SYMBOL_HPP
