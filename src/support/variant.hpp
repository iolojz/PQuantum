//
// Created by jayz on 25.08.19.
//

#ifndef PQUANTUM_VARIANT_HPP
#define PQUANTUM_VARIANT_HPP

#include <variant>

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
}
}

#endif //PQUANTUM_VARIANT_HPP
