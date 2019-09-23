//
// Created by jayz on 23.09.19.
//

#ifndef TAGD_MODELS_QUOTIENT_HPP
#define TAGD_MODELS_QUOTIENT_HPP

#include "tag_dispatch/tag_dispatch.hpp"

namespace tag_dispatch
{
namespace models
{
template<class UnderlyingTag, class StructureTag>
struct quotient_tag
{
	using underlying_tag = UnderlyingTag;
	using structure_tag = StructureTag;
};

template<class UnderlyingType, class StructureTag>
class quotient_wrapper
{
	UnderlyingType object;
public:
	using dispatch_tag = quotient_tag<tag_of_t < UnderlyingType>, StructureTag>;
	
	const UnderlyingType &representative( void ) const &
	{ return object; }
	
	UnderlyingType &representative( void ) &
	{ return object; }
	
	UnderlyingType &&representative( void ) &&
	{ return std::move( object ); }
};
}

template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
struct infer_tags<models::quotient_tag<DispatchTag, StructureTag>, void, Implementation>
{
	using dispatch_tag = DispatchTag;
	using structure_tag = StructureTag;
};

namespace impl
{
template<class DispatchTag, class StructureTag>
struct to<DispatchTag, models::quotient_tag<DispatchTag, StructureTag>, void>
{
	template<class Arg>
	decltype( auto ) apply( Arg &&arg )
	{
		return std::forward<Arg>( arg ).representative();
	}
};
}
}

#endif //TAGD_MODELS_QUOTIENT_HPP
