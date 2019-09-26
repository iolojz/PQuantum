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
template<class UnderlyingType, class StructureTag>
struct quotient_tag
{
};

template<class UnderlyingType, class StructureTag>
class quotient_wrapper
{
	UnderlyingType object;
public:
	using dispatch_tag = quotient_tag<UnderlyingType, StructureTag>;
	
	const UnderlyingType &representative( void ) const &
	{ return object; }
	
	UnderlyingType &representative( void ) &
	{ return object; }
	
	UnderlyingType &&representative( void ) &&
	{ return std::move( object ); }
};
}

template<class DispatchTag, class StructureTag, template<class, class> class Implementation>
struct structure_tag_for_function_dispatch<models::quotient_tag<DispatchTag, StructureTag>, Implementation>
{
	using type = models::quotient_tag<DispatchTag, StructureTag>;
};

namespace detail
{
template<class DispatchTag, class StructureTag, template<class, class> class Implementation, class ...Args>
struct quotient_post_action
{
private:
	using implementation = Implementation<DispatchTag, models::quotient_tag<DispatchTag, StructureTag>>;
	using result = decltype( implementation::apply( std::declval<Args>()... ));
	static constexpr bool pass_to_quotient = std::is_same_v<tag_of_t < result>, DispatchTag>;
public:
	using type = std::conditional_t<pass_to_quotient, impl::to<models::quotient_tag<DispatchTag, StructureTag>, DispatchTag, StructureTag>, impl::identity>;
};
}

template<class DispatchTag, class StructureTag, template<class, class> class Implementation, class ...Args>
struct function_traits_for_dispatch<models::quotient_tag<DispatchTag, StructureTag>, Implementation, Args...>
{
private:
	using tags = tags_for_function_dispatch<Implementation, Args...>;
public:
	using dispatch_tag = DispatchTag;
	using structure_tag = models::quotient_tag<DispatchTag, StructureTag>;
	using pre_action = impl::forward_or_convert<DispatchTag>;
	using post_action = detail::quotient_post_action<DispatchTag, StructureTag, Implementation, Args...>;
};

namespace impl
{
template<class DispatchTag, class StructureTag>
struct to<DispatchTag, models::quotient_tag<DispatchTag, StructureTag>, StructureTag>
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
