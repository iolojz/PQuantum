//
// Created by jayz on 05.02.20.
//

#ifndef PQUANTUM_PARSING_TREE_HELPERS_HPP
#define PQUANTUM_PARSING_TREE_HELPERS_HPP

#include <boost/type_index/ctti_type_index.hpp>

#include "x3_helpers.hpp"

#ifdef PQUANTUM_DEFINE_NAME_FOR_TREE_NODE
#error "PQUANTUM_DEFINE_NAME_FOR_TREE_NODE is already defined."
#endif
#define PQUANTUM_DEFINE_NAME_FOR_TREE_NODE(node_type) \
namespace PQuantum::parsing { \
template<class TreeTag> struct name_for_type_rule<support::tree::node_incarnation<node_type, TreeTag>> { \
    static std::string apply( void ) { \
        return "node<" + boost::typeindex::ctti_type_index::type_id<node_type>().pretty_name() + ">"; \
    } \
}; \
}


#endif //PQUANTUM_PARSING_TREE_HELPERS_HPP
