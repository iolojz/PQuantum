//
// Created by jayz on 25.08.19.
//

#ifndef PQUANTUM_VARIANT_HPP
#define PQUANTUM_VARIANT_HPP

#include "boost/variant.hpp"

namespace PQuantum::support {
namespace detail {
template<class ...Alternatives>
class to_std_visitor : public boost::static_visitor<void> {
	std::variant<Alternatives...> &std_v;
public:
	to_std_visitor(std::variant<Alternatives...> &v) : std_v{v} {}
	
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
}

#endif //PQUANTUM_VARIANT_HPP
