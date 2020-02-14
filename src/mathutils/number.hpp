//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_NUMBER_HPP
#define PQUANTUM_NUMBER_HPP

#include <boost/multiprecision/cpp_complex.hpp>

#include <cxxmath/cxxmath.hpp>

#include "support/tree.hpp"

namespace PQuantum::mathutils
{
struct number_tag {};

// FIXME: This should be a complex rational number!
//using number = boost::multiprecision::number<boost::multiprecision::complex_adaptor<boost::multiprecision::cpp_rational_backend>>;
using number = boost::multiprecision::cpp_complex<50>;

namespace model_number {
struct equal : cxxmath::supports_tag_helper<number_tag> {
	template<class N1, class N2>
	static constexpr decltype(auto) apply( N1 &&n1, N2 &&n2 ) {
		return std::forward<N1>( n1 ) == std::forward<N2>( n2 );
	}
};

struct add_assign : cxxmath::supports_tag_helper<number_tag> {
	template<class N1, class N2>
	static constexpr decltype(auto) apply( N1 &&n1, N2 &&n2 ) {
		return std::forward<N1>( n1 ) += std::forward<N2>( n2 );
	}
};

struct negate_in_place : cxxmath::supports_tag_helper<number_tag> {
	template<class N>
	static constexpr decltype(auto) apply( N &&n ) {
		return std::forward<N>( n ).backend().negate();
	}
};

struct zero {
	static auto apply( void ) {
		return number{ 0 };
	}
};

struct one {
	static auto apply( void ) {
		return number{ 1 };
	}
};

struct multiply_assign : cxxmath::supports_tag_helper<number_tag> {
	template<class N1, class N2>
	static constexpr decltype(auto) apply( N1 &&n1, N2 &&n2 ) {
		return std::forward<N1>( n1 ) *= std::forward<N2>( n2 );
	}
};
}
}

namespace cxxmath::impl {
template<> struct tag_of<PQuantum::mathutils::number> {
	using type = PQuantum::mathutils::number_tag;
};

/*template<> struct make<PQuantum::mathutils::number_tag> {
	template<class RealPart, class ImaginaryPart>
	static auto apply( RealPart &&rp, ImaginaryPart && ip = 0 ) {
		PQuantum::mathutils::number number;
		number.backend().real_data() = std::forward<RealPart>( rp );
		number.backend().imag_data() = std::forward<ImaginaryPart>( ip );
		return number;
	}
};*/

template<> struct default_set<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::set<
		PQuantum::mathutils::model_number::equal
	>;
};

template<> struct default_monoid<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::assignable_monoid<
	    PQuantum::mathutils::model_number::multiply_assign,
		PQuantum::mathutils::model_number::one,
		true_implementation
	>;
};

template<> struct default_group<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::assignable_group<
	    cxxmath::concepts::assignable_monoid<
			PQuantum::mathutils::model_number::multiply_assign,
			PQuantum::mathutils::model_number::zero,
			true_implementation
		>,
		PQuantum::mathutils::model_number::negate_in_place
	>;
};

template<> struct default_ring<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::ring<
		default_group_t<PQuantum::mathutils::number_tag>,
		default_monoid_t<PQuantum::mathutils::number_tag>
	>;
};
}

PQUANTUM_TREE_DEFINE_NODE_ARITY(mathutils::number, 0)

#endif //PQUANTUM_NUMBER_HPP
