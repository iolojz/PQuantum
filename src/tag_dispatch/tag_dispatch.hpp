//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_HPP
#define PQUANTUM_TAG_DISPATCH_HPP

#include <type_traits>

namespace PQuantum::tag_dispatch {
namespace detail {
template<class T>
struct has_type_dispatch_tag {
private:
	template<class V>
	static constexpr void test( decltype( std::declval<V::type>(), std::declval<int>()));
	
	template<typename V>
	static constexpr int test(char);

public:
	static constexpr bool value = std::is_void_v<decltype(test<std::decay_t<T>>(0))>;
};

template<class T, bool>
struct tag_of;

template<class T>
struct tag_of<T, true> {
	using type = typename T::dispatch_tag;
};

template<class T>
struct tag_of<T, false> {
	using type = std::decay_t<T>;
};
}

namespace impl {
template<class T>
struct tag_of
{
	using type = typename detail::tag_of<T, detail::has_type_dispatch_tag<T>::value>::type;
};
}

template<class T>
struct tag_of {
	using type = typename impl::tag_of<T>::type;
};
template<class T> using tag_of_t = typename tag_of<T>::type;
}

#ifdef PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION
#error "PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION is already defined."
#endif

#define PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION( function_name ) \
namespace PQuantum::tag_dispatch \
{ \
namespace functions { \
struct function_name { \
    template<class FunctionTag = void, class ...Args> \
    decltype(auto) apply( Args &&... args ) const \
    { \
        using dispatch_tag = PQuantum::tag_dispatch::tag_of_t<std::common_type_t<Args...>>; \
        return PQuantum::tag_dispatch::impl::function_name<dispatch_tag>::template apply<FunctionTag>( std::forward<Args>( args )... ); \
    } \
    \
    template<class ...Args> \
    decltype(auto) operator()( Args &&... args ) const \
    { return apply( std::forward<Args>( args )... ); } \
}; \
} \
\
static constexpr functions::function_name function_name; \
}

#ifdef PQUANTUM_ENABLE_IF_TAG_IS
#error "PQUANTUM_ENABLE_IF_TAG_IS is already defined."
#endif

#define PQUANTUM_ENABLE_IF_TAG_IS(type, tag) \
class = std::enable_if_t<std::is_same_v<PQuantum::tag_dispatch::tag_of_t<type>, tag>>

#endif //PQUANTUM_TAG_DISPATCH_HPP
