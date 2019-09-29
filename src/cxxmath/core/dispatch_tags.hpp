//
// Created by jayz on 24.09.19.
//

#ifndef CXXMATH_CORE_DISPATCH_TAGS_HPP
#define CXXMATH_CORE_DISPATCH_TAGS_HPP

#include <type_traits>

namespace cxxmath
{
namespace detail
{
template<class, class = void>
struct has_dispatch_tag : std::false_type
{
};

template<class T>
struct has_dispatch_tag<T, std::void_t<typename T::dispatch_tag>> : std::true_type
{
};

template<class T, bool>
struct tag_of;

template<class T>
struct tag_of<T, true>
{
	using type = typename T::dispatch_tag;
};

template<class T>
struct tag_of<T, false>
{
	using type = std::decay_t<T>;
};
}

namespace impl
{
template<class T>
struct tag_of
{
	using type = typename detail::tag_of<T, detail::has_dispatch_tag<T>::value>::type;
};

template<class Tag1, class Tag2>
struct common_tag
{
	using type = std::common_type_t<Tag1, Tag2>;
};
template<class Tag1, class Tag2> using common_tag_t = typename common_tag<Tag1, Tag2>::type;
}

template<class T>
struct tag_of
{
	using type = typename impl::tag_of<std::decay_t<T>>::type;
};
template<class T> using tag_of_t = typename tag_of<T>::type;

template<class ...Tags>
struct common_tag;
template<class ...Tags> using common_tag_t = typename common_tag<Tags...>::type;

template<class Tag>
struct common_tag<Tag>
{
	using type = Tag;
};

template<class Tag1, class Tag2, class ...Tags>
struct common_tag<Tag1, Tag2, Tags...>
{
	using type = common_tag_t<impl::common_tag_t<Tag1, Tag2>, Tags...>;
};
}

#ifdef CXXMATH_ENABLE_IF_TAG_IS
#error "CXXMATH_ENABLE_IF_TAG_IS is already defined."
#endif
#define CXXMATH_ENABLE_IF_TAG_IS( type, tag ) \
class = std::enable_if_t<std::is_same_v<::tag_dispatch::tag_of_t<type>, tag>>

#ifdef CXXMATH_DISABLE_IF_TAG_IS
#error "CXXMATH_DISABLE_IF_TAG_IS is already defined."
#endif
#define CXXMATH_DISABLE_IF_TAG_IS( type, tag ) \
class = std::enable_if_t<!std::is_same_v<::tag_dispatch::tag_of_t<type>, tag>>

#endif //CXXMATH_CORE_DISPATCH_TAGS_HPP
