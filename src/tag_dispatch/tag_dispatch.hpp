//
// Created by jayz on 03.09.19.
//

#ifndef TAGD_TAG_DISPATCH_HPP
#define TAGD_TAG_DISPATCH_HPP

#ifdef TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION
#error "TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION is already defined."
#endif
#define TAGD_DEFINE_TRAITED_DISPATCHED_FUNCTION( function_name ) \
namespace tag_dispatch \
{ \
template<class StructureTag = void> static constexpr detail::traited_function_object<StructureTag, impl::function_name> function_name; \
}

#ifdef TAGD_DEFINE_TAG_DISPATCHED_FUNCTION
#error "TAGD_DEFINE_TAG_DISPATCHED_FUNCTION is already defined."
#endif
#define TAGD_DEFINE_TAG_DISPATCHED_FUNCTION(function_name) \
namespace tag_dispatch \
{ \
template<class DispatchTag, class StructureTag = void> static constexpr detail::function_object<DispatchTag, StructureTag, impl::function_name> function_name; \
}

#ifdef TAGD_CONCEPT_IMPLEMENTS_FUNCTION
#error "TAGD_CONCEPT_IMPLEMENTS_FUNCTION is already defined."
#endif
#define TAGD_CONCEPT_IMPLEMENTS_FUNCTION( concept, structure_tag, function ) \
namespace tag_dispatch \
{ \
template<class DispatchTag> static constexpr std::enable_if_t< \
    concepts::concept<DispatchTag>::value, \
    concepts::concept<DispatchTag, structure_tag> \
> default_structure_tag( DispatchTag, impl_tag<impl::function> ) { return {}; } \
}

#ifdef TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION
#error "TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION is already defined."
#endif
#define TAGD_MODELLED_CONCEPT_IMPLEMENTS_FUNCTION( dispatch_tag, structure_tag, concept, function ) \
namespace tag_dispatch \
{ \
static constexpr concepts::concept<dispatch_tag, structure_tag> \
default_structure_tag( dispatch_tag, impl_tag<impl::function> ) { return {}; } \
}

#include "core/dispatch_tags.hpp"
#include "core/function_dispatch.hpp"

#include "concepts/boolean_lattice.hpp"
#include "concepts/group.hpp"
#include "concepts/makeable.hpp"
#include "concepts/monoid.hpp"
#include "concepts/ring.hpp"
#include "concepts/set.hpp"
#include "concepts/total_order.hpp"
#include "concepts/total_preorder.hpp"

#include "models/bool.hpp"
#include "models/clifford.hpp"
#include "models/free_monomial.hpp"
#include "models/free_polynomial.hpp"
#include "models/int.hpp"
#include "models/quotient.hpp"
#include "models/string_view.hpp"

#endif //TAGD_TAG_DISPATCH_HPP
