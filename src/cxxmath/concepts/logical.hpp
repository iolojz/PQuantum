//
// Created by jayz on 27.09.19.
//

#ifndef CXXMATH_CONCEPTS_LOGICAL_HPP
#define CXXMATH_CONCEPTS_LOGICAL_HPP

#include "boolean_lattice.hpp"

namespace cxxmath
{
namespace concepts
{
template<class BooleanLattice, class If>
struct logical
{
	using boolean_lattice_ = BooleanLattice;
	
	static constexpr auto and_ = boolean_lattice_::and_;
	static constexpr auto or_ = boolean_lattice_::or_;
	static constexpr auto not_ = boolean_lattice_::not_;
	
	static constexpr auto if_ = function_object_v<If>;
};
}

template<class DispatchTag, class BooleanLattice, class If>
struct models_concept<DispatchTag, concepts::logical<BooleanLattice, If>>
{
	using logical = concepts::logical<BooleanLattice, If>;
	using boolean_lattice = typename logical::boolean_lattice;
	static constexpr bool value = ( models_concept_v < DispatchTag, boolean_lattice > &&
																	logical::if_.template supports_tag<DispatchTag>());
};

template<class Tag>
struct default_logical : no_default_concept<Tag>
{
};
template<class Tag> using default_logical_t = typename default_logical<Tag>::type;

struct default_if__dispatch
{
	template<class Cond, class ...Args>
	constexpr decltype( auto ) operator()( Cond &&cond, Args &&... args ) const
	{
		using dispatch_tag = tag_of_t<Cond>;
		return default_logical_t<dispatch_tag>::if_( std::forward<Cond>( cond ), std::forward<Args>( args )... );
	}
} if_;
}

#endif //CXXMATH_CONCEPTS_LOGICAL_HPP
