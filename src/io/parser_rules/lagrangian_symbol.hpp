//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_LAGRANGIAN_SYMBOL_HPP
#define PQUANTUM_LAGRANGIAN_SYMBOL_HPP

#include "rule_for_symbol_fwd.hpp"
#include "template_helpers/variant.hpp"

#include <boost/spirit/home/x3.hpp>

namespace PQuantum {
namespace io {
namespace parser_rules
{
template<class ...Alternatives>
struct rule_for_symbol_impl<std::variant<Alternatives...>>
{
	template<class Context>
	constexpr auto operator()( Context &&c ) const
	{
		auto rule_def = (rule_for_symbol<Alternatives>( c ) | ...).operator[]( []( auto &&context ) {
			boost::spirit::x3::_val( context ) = template_helpers::to_std_variant<Alternatives...>(
			boost::spirit::x3::_attr( context ));
		} );
		return ( boost::spirit::x3::rule<std::variant<Alternatives...>, std::variant<Alternatives...>>{} = rule_def );
	}
};

struct int_tag;
struct string_id
{
};
struct index_core;

template<>
struct rule_for_symbol_impl<int>
{
	template<class StringToUUID>
	auto operator()( StringToUUID && ) const
	{
		return ( boost::spirit::x3::rule<int_tag, int>{} = boost::spirit::x3::int_ );
	}
};

template<>
struct rule_for_symbol_impl<string_id>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		auto rule_def = ( +boost::spirit::x3::char_ ).operator[]( [&uuid_gen]( auto &&context ) {
			boost::spirit::x3::_val( context ) = uuid_gen( boost::spirit::x3::_attr( context ));
		} );
		return ( boost::spirit::x3::rule<string_id, boost::uuids::uuid>{} = rule_def );
	}
};

template<>
struct rule_for_symbol_impl<index_core>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		auto rule_def = ( rule_for_symbol<int>( uuid_gen ) | rule_for_symbol<string_id>( uuid_gen )).operator[](
				[](auto &&context) {
			boost::spirit::x3::_val( context ) = template_helpers::to_std_variant<int, boost::uuids::uuid>(
			boost::spirit::x3::_attr( context ));
		} );
		return ( boost::spirit::x3::rule<index_core, std::variant<int, boost::uuids::uuid>>{} = rule_def );
	}
};

template<>
struct rule_for_symbol_impl<mathutils::spacetime_index>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		const auto variance_def = boost::spirit::x3::lit( "_" ).operator[]( []( auto &&context ) {
			boost::spirit::x3::_val(context).variance = mathutils::spacetime_index::index_variance::lower;
		} ) | boost::spirit::x3::lit( "^" ).operator[]( []( auto &&context )
		{
			boost::spirit::x3::_val(context).variance = mathutils::spacetime_index::index_variance::upper;
		} );
		const auto id_def = (
		( boost::spirit::x3::lit( "{" ) >> rule_for_symbol<index_core>( uuid_gen ) >> boost::spirit::x3::lit( "}" )) |
		rule_for_symbol<index_core>( uuid_gen ));
		return ( boost::spirit::x3::rule<mathutils::spacetime_index, mathutils::spacetime_index>{} = ( variance_def
		>> id_def ));
	}
};

template<>
struct rule_for_symbol_impl<mathutils::gamma_matrix>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		auto rule_def = boost::spirit::x3::lit( "\\gamma" )
		>> rule_for_symbol<mathutils::spacetime_index>( std::forward<StringToUUID>( uuid_gen ));
		return ( boost::spirit::x3::rule<mathutils::gamma_matrix, mathutils::gamma_matrix>{} = rule_def );
	}
};

template<>
struct rule_for_symbol_impl<mathutils::sigma_matrix>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		auto rule_def = boost::spirit::x3::lit( "\\sigma_{" ) >> rule_for_symbol<mathutils::spacetime_index>
			   ( std::forward<StringToUUID>( uuid_gen )) >> "," >> rule_for_symbol < mathutils::spacetime_index >
			   ( std::forward<StringToUUID>( uuid_gen ));
		return ( boost::spirit::x3::rule<mathutils::sigma_matrix, mathutils::sigma_matrix>{} = rule_def );
	}
};

template<>
struct rule_for_symbol_impl<mathutils::spacetime_derivative>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		auto rule_def = boost::spirit::x3::lit( "\\partial_" )
		>> rule_for_symbol<mathutils::spacetime_index>( std::forward<StringToUUID>( uuid_gen ));
		return ( boost::spirit::x3::rule<mathutils::spacetime_derivative, mathutils::spacetime_derivative>{} = rule_def );
	}
};

template<>
struct rule_for_symbol_impl<mathutils::dirac_operator>
{
	template<class Context>
	auto operator()( Context && ) const
	{
		auto rule_def = boost::spirit::x3::lit( "\\DiracOperator" ).operator[]( []( auto && ) {} );
		return ( boost::spirit::x3::rule<mathutils::dirac_operator, mathutils::dirac_operator>{} = rule_def );
	}
};

template<>
struct rule_for_symbol_impl<model::classical_field_id>
{
	template<class StringToUUID>
	auto operator()( StringToUUID &&uuid_gen ) const
	{
		auto rule_def = rule_for_symbol<string_id>( std::forward<StringToUUID>( uuid_gen ));
		return ( boost::spirit::x3::rule<model::classical_field_id, model::classical_field_id>{} = rule_def );
	}
};
}
}
}

#endif //PQUANTUM_LAGRANGIAN_SYMBOL_HPP
