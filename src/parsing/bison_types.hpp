#ifndef PQUANTUM_PARSING_BISON_TYPES_HPP
#define PQUANTUM_PARSING_BISON_TYPES_HPP

#include "mathutils/arithmetic.hpp"
#include "mathutils/atom_indices.hpp"
#include "mathutils/function_call.hpp"

#include <string>
#include <iostream>

namespace PQuantum::parsing {
static constexpr auto math_atom_arity_map = boost::hana::make_map(
	boost::hana::make_pair(
		boost::hana::type_c<mathutils::atom_with_optional_indices<std::string>>,
		boost::hana::int_c<0>
	)
);
static constexpr auto math_tree_arity_map = boost::hana::union_(
	boost::hana::union_(
		mathutils::function_call_arity_map<std::string>,
		mathutils::arithmetic_arity_map
	),
	math_atom_arity_map
);

using math_tree = cxxmath::typesafe_tree<decltype(math_tree_arity_map)>;
using function_call_node = cxxmath::typesafe_tree_node<mathutils::function_call<std::string>, math_tree>;

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC is already defined"
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC( name ) \
[[maybe_unused]] static math_tree make_arithmetic_ ## name( math_tree &&n1, math_tree &&n2 ) { \
    if( cxxmath::holds_node<mathutils::name>( n1 ) ) { \
        auto n1_ = cxxmath::get_node<mathutils::name>( std::move( n1 ) ); \
        \
        if( cxxmath::holds_node<mathutils::name>( n2 ) ) { \
            auto n2_ = cxxmath::get_node<mathutils::name>( std::move( n2 ) ); \
            \
            n1_.children.insert( n1_.children.end(), std::make_move_iterator( n2_.children.begin() ), \
                                 std::make_move_iterator( n2_.children.end() ) ); \
        } else \
            n1_.children.emplace_back( std::move( n2 ) ); \
        \
        return n1_; \
    } else if( cxxmath::holds_node<mathutils::name>( n2 ) ) { \
        auto n2_ = cxxmath::get_node<mathutils::name>( std::move( n2 ) ); \
        n2_.children.emplace_back( std::move( n1 ) ); \
        std::rotate( std::begin(n2_.children), --std::end(n2_.children), std::end(n2_.children) ); \
        return n2_; \
    } \
    \
    return { mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

#ifdef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
#error "PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC is already defined."
#endif
#define PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC( name ) \
[[maybe_unused]] static math_tree make_arithmetic_ ## name( math_tree &&n1, math_tree &&n2 ) { \
    return { mathutils::name{}, std::move( n1 ), std::move( n2 ) }; \
}

PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC( sum )
PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC( product )

PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC( difference )
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC( quotient )
PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC( power )

[[maybe_unused]] static math_tree make_arithmetic_negation( math_tree &&n ) {
	return { mathutils::negation{}, std::move( n ) };
}

[[maybe_unused]] static math_tree make_arithmetic_parentheses( math_tree &&n ) {
	return { mathutils::parentheses{}, std::move( n ) };
}

#undef PQUANTUM_PARSING_DEFINE_MAKE_ASSOCIATIVE_ARITHMETIC
#undef PQUANTUM_PARSING_DEFINE_MAKE_BINARY_ARITHMETIC
}

#endif /* PQUANTUM_PARSING_BISON_TYPES_HPP */
