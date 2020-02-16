//
// Created by jayz on 16.01.20.
//

#ifndef PQUANTUM_MATHUTILS_ARITHMETIC_HPP
#define PQUANTUM_MATHUTILS_ARITHMETIC_HPP

#include "support/tree.hpp"

#include <boost/hana.hpp>

#include <string>

namespace PQuantum::mathutils {
struct power {};
struct product {};
struct quotient {};
struct sum {};
struct difference {};
struct negation {};

static constexpr auto ops_by_precedence = boost::hana::tuple_t<power, product, quotient, sum, difference>;

struct arithmetic_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<
		power, product, quotient, sum, difference, negation
	>;
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::power, 2 )
PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::product, runtime_arity )
PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::quotient, 2 )
PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::sum, runtime_arity )
PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::difference, 2 )
PQUANTUM_TREE_DEFINE_NODE_ARITY( mathutils::negation, 1 )

#ifdef PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR
#error "PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR is already defined"
#endif
#define PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( op_name, op_str ) \
namespace PQuantum::support::tree { \
template<class TreeNode> \
std::ostream &operator<<( std::ostream &os, const node_incarnation<mathutils::op_name, TreeNode> &ni ) { \
    if( ni.children.empty() ) \
        throw std::runtime_error( "operator \"" op_str "\" has no children" ); \
    \
    auto it = std::begin( ni.children ); \
    os << *it; \
    for( ++it; it != std::end( ni.children ); ++it ) \
        os << " " op_str " " << *it; \
    return os; \
} \
}

PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( power, "^" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( product, "*" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( quotient, "/" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( sum, "+" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( difference, "-" )

#undef PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR

namespace PQuantum::support::tree {
template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation <mathutils::negation, TreeNode> &ni ) {
	return os << "- " << ni.children.front();
}
}

#endif //PQUANTUM_MATHUTILS_ARITHMETIC_HPP
