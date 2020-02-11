//
// Created by jayz on 10.02.20.
//

#ifndef PQUANTUM_PARSING_MATH_TYPES_HPP
#define PQUANTUM_PARSING_MATH_TYPES_HPP

#include "mathutils/arithmetic.hpp"

#include <string>

#include "support/tree.hpp"

namespace PQuantum::parsing {
struct atom {
	std::string data;
};

using index_list = std::vector<atom>;

struct index_spec {
	index_list lower, upper;
};

struct atom_with_optional_indices {
	atom name;
	index_spec indices;
};

struct function_call {
	atom name;
};

struct functions_and_indexed_atoms_node_traits {
	static constexpr auto node_data_types = boost::hana::tuple_t<atom_with_optional_indices, function_call>;
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(parsing::atom_with_optional_indices, 0)
PQUANTUM_TREE_DEFINE_NODE_ARITY(parsing::function_call, runtime_arity)

namespace PQuantum::parsing {
using tree_tag = support::tree::tree_tag<
	functions_and_indexed_atoms_node_traits,
	mathutils::arithmetic_node_traits
>;
using tree_node = typename tree_tag::node;

namespace detail {
using arithmetic_tree_node = support::tree::tree_node<
	mathutils::arithmetic_node_traits
>;
}

using function_call_node = support::tree::node_incarnation<function_call, tree_tag>;
using arithmetic_node = typename support::tree::rebind_tree_tag<tree_tag, detail::arithmetic_tree_node>::type;

tree_node make_atom_with_optional_indices( atom_with_optional_indices &&indexed_a ) {
	return support::tree::node_incarnation<atom_with_optional_indices, tree_tag>{ std::move( indexed_a ) };
}

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC is already defined"
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(name) \
static tree_node make_arithmetic_ ## name( tree_node &&n1, tree_node &&n2 ) { \
	if( support::tree::holds_node_incarnation<mathutils::name>( n1 ) ) { \
		auto n1_ = support::tree::get_node_incarnation<mathutils::name>( std::move( n1 ) ); \
		\
		if( support::tree::holds_node_incarnation<mathutils::name>( n2 ) ) { \
			auto n2_ = support::tree::get_node_incarnation<mathutils::name>( std::move( n2 ) ); \
			\
			n1_.children.insert( n1_.children.end(), std::make_move_iterator( n2_.children.begin() ), \
								 std::make_move_iterator( n2_.children.end() ) ); \
		} else \
			n1_.children.emplace_back( std::move( n2 ) ); \
		\
		return n1_; \
	} else if( support::tree::holds_node_incarnation<mathutils::name>( n2 ) ) { \
		auto n2_ = support::tree::get_node_incarnation<mathutils::name>( std::move( n2 ) ); \
		n2_.children.emplace_back( std::move( n1 ) ); \
		return n2_; \
	} \
	\
	return support::tree::node_incarnation<mathutils::name, tree_tag>{ mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC is already defined."
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(name) \
static tree_node make_arithmetic_ ## name( tree_node &&n1, tree_node &&n2 ) { \
	return support::tree::node_incarnation<mathutils::name, tree_tag>{ mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(sum)
PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(product)

PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(difference)
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(quotient)
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(power)

static tree_node make_arithmetic_negation( tree_node &&n ) { \
	return support::tree::node_incarnation<mathutils::negation, tree_tag>{ mathutils::negation{}, std::move( n ) }; \
}

#undef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#undef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
}

#endif //PQUANTUM_PARSING_MATH_TYPES_HPP
