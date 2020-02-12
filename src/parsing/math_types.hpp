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
	std::string name;
	
	atom( void ) = default;
	atom( const atom & ) = default;
	atom( atom && ) = default;
	
	atom &operator=( const atom & ) = default;
	atom &operator=( atom && ) = default;
	
	template<class Arg1, class ...Args, class = std::enable_if_t<!std::is_same_v<std::decay_t<Arg1>, atom>>>
	atom( Arg1 &&arg1, Args &&...args )
	: name{ std::forward<Arg1>( arg1 ), std::forward<Args>( args )... } {}
};

using index_list = std::vector<atom>;

struct index_spec {
	index_list lower, upper;
};

struct atom_with_optional_indices {
	decltype(atom::name) name;
	index_spec indices;
};

struct function_call {
	decltype(atom::name) name;
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

[[maybe_unused]] static tree_node make_atom_with_optional_indices( atom_with_optional_indices &&indexed_a ) {
	return support::tree::node_incarnation<atom_with_optional_indices, tree_tag>{ std::move( indexed_a ) };
}

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC is already defined"
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(name) \
[[maybe_unused]] static tree_node make_arithmetic_ ## name( tree_node &&n1, tree_node &&n2 ) { \
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
[[maybe_unused]] static tree_node make_arithmetic_ ## name( tree_node &&n1, tree_node &&n2 ) { \
	return support::tree::node_incarnation<mathutils::name, tree_tag>{ mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(sum)
PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC(product)

PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(difference)
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(quotient)
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC(power)

[[maybe_unused]] static tree_node make_arithmetic_negation( tree_node &&n ) { \
	return support::tree::node_incarnation<mathutils::negation, tree_tag>{ mathutils::negation{}, std::move( n ) }; \
}

#undef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#undef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
}

namespace PQuantum::support::tree {
[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const parsing::index_spec &is ) {
	if( is.lower.empty() == false ) {
		os << "_{" << is.lower.front().name;
		for( auto it = ++(is.lower.begin()); it != is.lower.end(); ++it )
			os << ", " << it->name;
		os << "}";
	}
	
	if( is.upper.empty() == false ) {
		os << "^{" << is.lower.front().name;
		for( auto it = ++(is.upper.begin()); it != is.upper.end(); ++it )
			os << ", " << it->name;
		os << "}";
	}
	
	return os;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<parsing::atom_with_optional_indices, TreeNode> &ni ) {
	return os << ni.data.name << ni.data.indices;
}

template<class TreeNode>
std::ostream &operator<<( std::ostream &os, const node_incarnation<parsing::function_call, TreeNode> &ni ) {
	return os << ni.data.name << "{";
	if( ni.children.empty() == false ) {
		os << ni.children.front();
		for( auto it = ++(ni.children.begin()); it != ni.children.end(); ++it )
			os << ", " << *it;
	}
	return os << "}";
}
}

#endif //PQUANTUM_PARSING_MATH_TYPES_HPP
