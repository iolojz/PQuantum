//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_HPP
#define PQUANTUM_TAG_DISPATCH_HPP

#include <type_traits>
#include <tuple>

#include "std_tags.hpp"

namespace PQuantum::tag_dispatch {
namespace detail {
template<class T>
struct has_type_dispatch_tag {
private:
	template<class T>
	static constexpr void test(decltype(typename V::type(), int{}));
	
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

// Single tag
template<template<class ...> class Template, class Tag>
struct dispatch {
	using type = Template<Tag>;
};

// Multiple tags
template<template<class ...> class Template, class ...Tags>
struct dispatch<Template, std::tuple<Tags...>> {
	using type = Template<Tags...>;
};

template<template<class ...> class Template, class Tag> using dispatch_t = typename dispatch<Template, Tag>::type;
}

namespace impl {
template<class T>
class tag_of {
public:
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

#define PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(name)
template<class ...Args> static constexpr auto name = PQuantum::tag_dispatch::detail::dispatch_t<PQuantum::tag_dispatch::impl::name, PQuantum::tag_dispatch::tag_of_t<std::common_type_t<Args...>>>{}

#ifdef PQUANTUM_ENABLE_IF_TAG_IS
#error "PQUANTUM_ENABLE_IF_TAG_IS is already defined."
#endif

#define PQUANTUM_ENABLE_IF_TAG_IS(type, tag) \
class = std::enable_if_t<std::is_same_v<PQuantum::tag_dispatch::tag_of_t<type>, tag>>

#endif //PQUANTUM_TAG_DISPATCH_HPP
