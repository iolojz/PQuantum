//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_MAKE_HPP
#define PQUANTUM_MAKE_HPP

#include "tag_dispatch.hpp"

namespace PQuantum::tag_dispatch
{
namespace impl
{
template<class Tag>
struct make
{
	template<class FunctionTag = void, class ...Args>
	static decltype( auto ) apply( Args &&...args )
	{ return Tag{ std::forward<Args>( args )... }; }
};
}

namespace functions
{
template<class DispatchTag>
struct make
{
	template<class FunctionTag = void, class ...Args>
	decltype( auto ) apply( Args &&... args ) const
	{
		return PQuantum::tag_dispatch::impl::make<DispatchTag>::template apply<FunctionTag>(
		std::forward<Args>( args )... );
	}
	
	template<class ...Args>
	decltype( auto ) operator()( Args &&... args ) const
	{ return apply( std::forward<Args>( args )... ); }
};
}

template<class DispatchTag> static constexpr functions::make<DispatchTag> make;
}

#endif //PQUANTUM_MAKE_HPP
