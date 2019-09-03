//
// Created by jayz on 03.09.19.
//

#ifndef PQUANTUM_STD_TAGS_HPP
#define PQUANTUM_STD_TAGS_HPP

#include "type_dispatch_fwd.hpp"

#include <variant>

#ifdef PQUANTUM_TAG_STD_TEMPLATE
#error "PQUANTUM_TAG_STD_TEMPLATE already defined"
#endif

#define PQUANTUM_TAG_STD_TEMPLATE(temp) \
struct std_ ## temp ## _tag {}; \
template<class ...TemplateArgs> struct tag_of_impl<std::temp<TemplateArgs...>> \
{ \
    using type = std_ ## temp ## _tag; \
};

namespace PQuantum {
namespace type_dispatch {
PQUANTUM_TAG_STD_TEMPLATE(variant)
PQUANTUM_TAG_STD_TEMPLATE(vector)
PQUANTUM_TAG_STD_TEMPLATE(map)
}
}

#undef PQUANTUM_TAG_STD_TEMPLATE

namespace

#endif //PQUANTUM_STD_TAGS_HPP
