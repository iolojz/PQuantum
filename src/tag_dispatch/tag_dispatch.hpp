//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_TAG_DISPATCH_HPP
#define PQUANTUM_TAG_DISPATCH_HPP

#include <type_traits>

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/pop_front.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>

namespace PQuantum::tag_dispatch {
namespace detail {
template<class, class = std::void_t<>>
struct has_dispatch_tag : std::false_type {
};

template<class T>
struct has_dispatch_tag<T, std::void_t<typename T::dispatch_tag>> : std::true_type {
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
namespace detail {
struct no_impl {
};
}

template<class T>
struct tag_of
{
	using type = typename tag_dispatch::detail::tag_of<T, tag_dispatch::detail::has_dispatch_tag<T>::value>::type;
};
}

using impl::tag_of;
template<class T> using tag_of_t = typename tag_of<T>::type;

namespace concepts {
namespace detail {
template<class Tag, template<class> class Concept>
static constexpr std::false_type is_synthesized(long);

static constexpr int magic_synthesized_query = 42;
}

template<class Tag, template<class> class Concept>
static constexpr bool is_synthesized(void) {
	return decltype(detail::is_synthesized<Tag, Concept>(detail::magic_synthesized_query))::value;
}

template<class Tag, template<class> class Concept>
static constexpr bool implements(void) {
	return Concept<Tag>::value || is_synthesized<Tag, Concept>();
}
}
}

#ifdef PQUANTUM_TAG_DISPATCH_ASSERT_CONCEPT
#error "PQUANTUM_TAG_DISPATCH_ASSERT_CONCEPT is already defined."
#endif

#define PQUANTUM_TAG_DISPATCH_ASSERT_CONCEPT(r, tag, concept) \
static_assert( concepts::concept<tag>::value, "Concept \" BOOST_PP_STRINGIZE(concept) "\" not implemented!" );

#ifdef PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION
#error "PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION is already defined."
#endif

// ( function_name, concepts... )
#define PQUANTUM_DEFINE_TAG_DISPATCHED_FUNCTION(...) \
namespace PQuantum::tag_dispatch \
{ \
namespace functions { \
struct BOOST_PP_SEQ_HEAD(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) { \
    template<class FunctionTag = void, class ...Args> \
    decltype(auto) apply( Args &&... args ) const \
    { \
        using dispatch_tag = PQuantum::tag_dispatch::tag_of_t<std::common_type_t<Args...>>; \
        BOOST_PP_SEQ_FOR_EACH(PQUANTUM_TAG_DISPATCH_ASSERT_CONCEPT, dispatch_tag, BOOST_PP_SEQ_POP_FRONT(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))) \
        \
        using PQuantum::tag_dispatch::impl::BOOST_PP_SEQ_HEAD(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))<dispatch_tag>::apply

return

PQuantum::tag_dispatch::impl::BOOST_PP_SEQ_HEAD(
		BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))<dispatch_tag>::template apply<FunctionTag> ( \
				std::forward<Args>(args)...); \
    } \
    \
    template<class ...Args> \
    decltype(auto) operator()( Args &&... args ) const \
    { return apply( std::forward<Args>( args )... ); } \
}; \
} \
\

static constexpr functions::BOOST_PP_SEQ_HEAD(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) BOOST_PP_SEQ_HEAD(
		BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));
\
}

#ifdef PQUANTUM_ENABLE_IF_TAG_IS
#error "PQUANTUM_ENABLE_IF_TAG_IS is already defined."
#endif

#define PQUANTUM_ENABLE_IF_TAG_IS(type, tag) \
class = std::enable_if_t<std::is_same_v<PQuantum::tag_dispatch::tag_of_t<type>, tag>>

#ifdef PQUANTUM_ENABLE_IF_TAG_IMPLEMENTS_CONCEPT
#error "PQUANTUM_ENABLE_IF_TAG_IMPLEMENTS_CONCEPT is already defined."
#endif

#define PQUANTUM_ENABLE_IF_TAG_IMPLEMENTS_CONCEPT(tag, concept) \
class = std::enable_if_t<PQuantum::tag_dispatch::concepts::implements<tag, concept>()>

#ifdef PQUANTUM_ENABLE_IF_CONCEPT_IS
#error "PQUANTUM_ENABLE_IF_CONCEPT_IS is already defined."
#endif

#define PQUANTUM_ENABLE_IF_CONCEPT_IS(concept1, concept2) \
class = std::enable_if_t<std::is_same_v<concept1, concept2>>

#ifdef PQUANTUM_TAG_DISPATCH_CONCEPT_REFINES_CONCEPT
#error "PQUANTUM_TAG_DISPATCH_CONCEPT_REFINES_CONCEPT is already defined."
#endif

#define PQUANTUM_TAG_DISPATCH_CONCEPT_REFINES_CONCEPT(refined_concept, base_concept) \
namespace PQuantum::tag_dispatch::concepts::detail { \
template<class Tag, template<class> class Concept, \
    PQUANTUM_ENABLE_IF_TAG_IMPLEMENTS_CONCEPT(Tag, refined_concept), \
    PQUANTUM_ENABLE_IF_CONCEPT_IS(Concept<Tag>,base_concept<Tag>) \
> \
static constexpr std::true_type is_synthesized(int); \
}

#ifdef PQUANTUM_DEFINE_DEFAULT_TAG_DISPATCHED_FUNCTION
#error "PQUANTUM_DEFINE_DEFAULT_TAG_DISPATCHED_FUNCTION(equal) is already defined."
#endif

#define PQUANTUM_DEFINE_DEFAULT_TAG_DISPATCHED_FUNCTION(function_name) \
namespace PQuantum::tag_dispatch::impl { \
template<class Tag> struct function_name : detail::no_impl {}; \
}

#endif //PQUANTUM_TAG_DISPATCH_HPP
