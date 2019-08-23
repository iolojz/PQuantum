//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_LAGRANGIAN_SYMBOL_HPP
#define PQUANTUM_LAGRANGIAN_SYMBOL_HPP

#include <boost/spirit/home/x3.hpp>

namespace PQuantum {
namespace io {
namespace lagrangian_symbol_parser
{
namespace x3 = boost::spirit::x3;

const x3::rule<class string_id, boost::uuids::uuid> string_id = "string_id";

const x3::rule<mathutils::spacetime_index::index_variance, mathutils::spacetime_index::index_variance> index_variance = "index_variance";
const auto index_variance_def = (boost::spirit::x3::lit("_") >> x3::attr(mathutils::spacetime_index::index_variance::lower)) |
					  (boost::spirit::x3::lit("_") >> x3::attr(mathutils::spacetime_index::index_variance::upper));
BOOST_SPIRIT_DEFINE( index_variance )

const x3::rule<class index_core, std::variant<int, boost::uuids::uuid>> index_core = "index_core";
const auto index_core_def = x3::int_ | string_id;
BOOST_SPIRIT_DEFINE( index_core )

const x3::rule<mathutils::spacetime_index, mathutils::spacetime_index> spacetime_index = "spacetime_index";
const auto spacetime_index_def = index_variance >> (x3::lit("{") >> index_core >> x3::lit("}")) | index_core;
BOOST_SPIRIT_DEFINE( spacetime_index )


}

/*
namespace parser_rules {
namespace detail {
template<class Symbol>
struct rule_for_symbol_base : public boost::spirit::x3::rule<rule_for_symbol_base<Symbol>, Symbol>
{
	const string_to_uuid &uuid_gen;
	
	using base_rule = boost::spirit::x3::rule<rule_for_symbol_base<Symbol>, Symbol>;
	rule_for_symbol_base( const char *str, const string_to_uuid &u ) : base_rule{ str }, uuid_gen{ u }
	{}
};
}

template<class Symbol> class rule_for_symbol;

#ifdef PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE
#error "PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE macro already defined"
#endif

#define PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(symbol) \
template<class Iterator, class Context, class Attribute> \
inline bool parse_rule( const typename detail::rule_for_symbol_base<symbol>::base_rule &rule, \
    Iterator& first, const Iterator &last, \
    const Context &context, Attribute& attr )\
{ return rule.definition().parse( first, last, context, boost::spirit::x3::unused, attr ); }

template<>
class rule_for_symbol<mathutils::spacetime_index>
: public detail::rule_for_symbol_base<mathutils::spacetime_index>
{
	struct assign_variance_lower {
		template<class Context>
		void operator()(const Context &context) const {
			boost::spirit::x3::_val(context).variance = mathutils::spacetime_index::index_variance::lower;
		}
	};
	
	struct assign_variance_upper {
		template<class Context>
		void operator()(const Context &context) const {
			boost::spirit::x3::_val(context).variance = mathutils::spacetime_index::index_variance::upper;
		}
	};
	
	struct assign_id {
		const string_to_uuid &uuid_gen;
		
		template<class Context>
		void operator()(const Context &context) const {
			std::visit(assign_id_impl{uuid_gen, boost::spirit::x3::_val(context).id},
					   boost::spirit::x3::_attr(context));
		}
	};
	
	struct assign_id_impl {
		const string_to_uuid &uuid_gen;
		std::variant<int, boost::uuids::uuid> &id;
		
		void operator()(int i) const { id = i; }
		
		void operator()(const std::string &str) const { id = uuid_gen(str); }
	};

public:
	rule_for_symbol( const string_to_uuid &u )
	: detail::rule_for_symbol_base<mathutils::spacetime_index>{ "rule_for_symbol<spacetime_index>", u }
	{}
	
	auto definition(void) const {
		const auto variance_def = boost::spirit::x3::lit("_")[assign_variance_lower{}] |
								  boost::spirit::x3::lit("^")[assign_variance_upper{}];
		const auto id_def = ((boost::spirit::x3::lit("{") >> (boost::spirit::x3::int_ | +boost::spirit::x3::char_)
														  >> boost::spirit::x3::lit("}")) |
							 (boost::spirit::x3::int_ | +boost::spirit::x3::char_))[assign_id{uuid_gen}];
		
		return variance_def >> id_def;
	}
};

template<>
class rule_for_symbol<mathutils::gamma_matrix>
: public detail::rule_for_symbol_base<mathutils::gamma_matrix> {
public:
	rule_for_symbol( const string_to_uuid &u )
	: detail::rule_for_symbol_base<mathutils::gamma_matrix>{ "rule_for_symbol<mathutils::gamma_matrix>", u }
	{}
	
	auto definition(void) const {
		return boost::spirit::x3::lit("\\gamma") >> rule_for_symbol<mathutils::spacetime_index>{uuid_gen};
	}
};

template<>
class rule_for_symbol<mathutils::sigma_matrix>
: public detail::rule_for_symbol_base<mathutils::sigma_matrix> {
public:
	rule_for_symbol( const string_to_uuid &u )
			: detail::rule_for_symbol_base<mathutils::sigma_matrix>{ "rule_for_symbol<mathutils::gamma_matrix>", u } {}
	
	auto definition(void) const {
		return boost::spirit::x3::lit("\\sigma_{") >> rule_for_symbol<mathutils::spacetime_index>{uuid_gen} >> ","
												   >> rule_for_symbol<mathutils::spacetime_index>{uuid_gen};
	}
};

template<>
class rule_for_symbol<mathutils::spacetime_derivative>
: public detail::rule_for_symbol_base<mathutils::spacetime_derivative> {
public:
	rule_for_symbol( const string_to_uuid &u )
	: detail::rule_for_symbol_base<mathutils::spacetime_derivative>{ "rule_for_symbol<mathutils::gamma_matrix>", u } {}
	
	auto definition(void) const {
		return boost::spirit::x3::lit("\\partial_") >> rule_for_symbol<mathutils::spacetime_index>{uuid_gen};
	}
};

template<>
class rule_for_symbol<mathutils::dirac_operator>
: public detail::rule_for_symbol_base<mathutils::dirac_operator> {
public:
	rule_for_symbol( const string_to_uuid &u )
	: detail::rule_for_symbol_base<mathutils::dirac_operator>{ "rule_for_symbol<mathutils::dirac_operator>", u }
	{}
	
	auto definition(void) const { return boost::spirit::x3::lit("\\DiracOperator"); }
};

template<>
class rule_for_symbol<model::classical_field_id>
: public detail::rule_for_symbol_base<model::classical_field_id> {
	struct assign_id {
		const string_to_uuid &uuid_gen;
		
		template<class Context>
		void operator()(const Context &context) const {
			boost::spirit::x3::_val(context) = boost::spirit::x3::_attr(context);
		}
	};

public:
	rule_for_symbol(const string_to_uuid &u)
			: detail::rule_for_symbol_base<model::classical_field_id>{ "rule_for_symbol<model::classical_field_id>", u }
			{}
	
	auto definition(void) const { return (+boost::spirit::x3::char_)[assign_id{uuid_gen}]; }
};

template<class ...Alternatives>
class rule_for_symbol<std::variant<Alternatives...>>
: public detail::rule_for_symbol_base<std::variant<Alternatives...>> {
	using impl_base = detail::rule_for_symbol_base<std::variant<Alternatives...>>;
public:
	rule_for_symbol(const string_to_uuid &u)
			: impl_base{ "rule_for_symbol<std::variant<...>>", u }
			{}
	
	auto definition(void) const { return (rule_for_symbol<Alternatives>{impl_base::uuid_gen} | ...); }
};

PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(mathutils::spacetime_index)
PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(mathutils::gamma_matrix)
PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(mathutils::sigma_matrix)
PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(mathutils::spacetime_derivative)
PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(mathutils::dirac_operator)
PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(model::classical_field_id)
PQUANTUM_SYNTHESIZE_SPIRIT_X3_PARSE_RULE(model::lagrangian_symbol)
}
 
 */
}
}

#endif //PQUANTUM_LAGRANGIAN_SYMBOL_HPP
