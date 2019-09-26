//
// Created by jayz on 24.09.19.
//

#ifndef TAGD_CORE_IDENTITY_HPP
#define TAGD_CORE_IDENTITY_HPP

namespace tag_dispatch::impl
{
struct identity
{
	template<class Arg>
	static constexpr std::conditional_t<std::is_rvalue_reference_v<Arg &&>, std::decay_t<Arg &&>, Arg &&>
	apply( Arg &&arg )
	{ return std::forward<Arg>( arg ); }
};
}

#endif //TAGD_CORE_IDENTITY_HPP
