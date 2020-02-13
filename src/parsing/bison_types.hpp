#ifndef PQUANTUM_PARSING_BISON_TYPES_HPP
#define PQUANTUM_PARSING_BISON_TYPES_HPP

#include "mathutils/arithmetic.hpp"
#include "mathutils/string_atom.hpp"
#include "mathutils/atom_indices.hpp"
#include "mathutils/function_call.hpp"

#include <string>

#include "support/tree.hpp"

namespace PQuantum::parsing {
using math_tree_tag = support::tree::tree_tag<
	mathutils::function_call_node_traits<mathutils::string_atom>,
	mathutils::indexed_atoms_node_traits<mathutils::string_atom>,
	mathutils::arithmetic_node_traits
>;
using math_tree_node = typename math_tree_tag::node;

using function_call_node = support::tree::node_incarnation<mathutils::function_call<mathutils::string_atom>, math_tree_tag>;

[[maybe_unused]] static math_tree_node make_atom_with_optional_indices( mathutils::atom_with_optional_indices<mathutils::string_atom> &&indexed_a ) {
	return support::tree::node_incarnation<mathutils::atom_with_optional_indices<mathutils::string_atom>, math_tree_tag>{ std::move( indexed_a ) };
}

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC is already defined"
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(name) \
[[maybe_unused]] static math_tree_node make_arithmetic_ ## name( math_tree_node &&n1, math_tree_node &&n2 ) { \
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
	return support::tree::node_incarnation<mathutils::name, math_tree_tag>{ mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC is already defined."
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(name) \
[[maybe_unused]] static math_tree_node make_arithmetic_ ## name( math_tree_node &&n1, math_tree_node &&n2 ) { \
	return support::tree::node_incarnation<mathutils::name, math_tree_tag>{ mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(sum)
PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(product)

PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(difference)
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(quotient)
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(power)

[[maybe_unused]] static math_tree_node make_arithmetic_negation( math_tree_node &&n ) { \
	return support::tree::node_incarnation<mathutils::negation, math_tree_tag>{ mathutils::negation{}, std::move( n ) }; \
}

#undef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#undef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
}

#endif /* PQUANTUM_PARSING_BISON_TYPES_HPP */
