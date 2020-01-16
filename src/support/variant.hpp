//
// Created by jayz on 25.08.19.
//

#ifndef PQUANTUM_VARIANT_HPP
#define PQUANTUM_VARIANT_HPP

#include <variant>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace PQuantum::support {
namespace detail {
template<class ...Alternatives>
class to_std_visitor : public boost::static_visitor<void> {
	std::variant<Alternatives...> &std_v;
public:
	to_std_visitor( std::variant<Alternatives...> &v ) : std_v{ v } {}
	
	template<class Alternative>
	void operator()( Alternative &&a ) const
	{
		std_v = std::forward<Alternative>( a );
	}
};
}

template<class ...Alternatives, class BoostVariant>
std::variant<Alternatives...> to_std_variant( BoostVariant &&v )
{
	std::variant<Alternatives...> std_v;
	detail::to_std_visitor<Alternatives...> visitor{ std_v };
	
	boost::apply_visitor( visitor, std::forward<BoostVariant>( v ));
	return std_v;
}

template<class Tuple>
struct std_variant_over_tuple_types;
template<class ...Types>
struct std_variant_over_tuple_types<std::tuple<Types...>> {
	using type = std::variant<Types...>;
};

namespace parsing {
template<class ...Alternatives>
struct rule_for_impl<std::variant<Alternatives...>> {
	static constexpr const char *name = "std_variant";
	
	template<class ...Args>
	static constexpr auto apply( Args &&...args ) {
		static_assert( sizeof...( Alternatives ) != 0 );
		
		if constexpr( sizeof...( Alternatives ) == 1 ) {
			return rule_for<Alternatives...>( std::forward<Args>( args )... );
		} else
			return (rule_for<Alternatives>( args... ) | ...).operator[]( []( auto &&context ) {
				boost::spirit::x3::_val( context ) = support::to_std_variant<Alternatives...>(
					boost::spirit::x3::_attr( context )
				);
			} );
	}
};

template<class ...Alternatives>
struct rule_for_impl<boost::variant<Alternatives...>> {
	static constexpr const char *name = "boost_variant";
	
	template<class ...Args>
	static constexpr auto apply( Args &&...args ) {
		static_assert( sizeof...( Alternatives ) != 0 );
		return (rule_for<typename boost::unwrap_recursive<Alternatives>::type>( args... ) | ...);
	}
};
}
}

#endif //PQUANTUM_VARIANT_HPP
