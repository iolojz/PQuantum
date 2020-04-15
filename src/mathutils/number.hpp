//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_NUMBER_HPP
#define PQUANTUM_NUMBER_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <cxxmath/cxxmath.hpp>

namespace PQuantum::mathutils {
struct number_tag {};

class number {
	boost::multiprecision::cpp_rational real, imag;
public:
	using cxxmath_dispatch_tag = number_tag;
	
	number( void ) = default;
	number( number && ) = default;
	number( const number & ) = default;
	
	template<
	    class Real,
	    class Imag = int,
	    class = std::enable_if_t<std::is_constructible_v<decltype(real), Real &&>>,
		class = std::enable_if_t<std::is_constructible_v<decltype(imag), Imag &&>>
	> number( Real &&r, Imag &&i = 0 )
	: real( std::forward<Real>( r ) ), imag( std::forward<Imag>( i ) ) {}
	
	number &operator=( number && ) = default;
	number &operator=( const number & ) = default;
	
	auto &real_data( void ) { return real; }
	const auto &real_data( void ) const { return real; }
	
	auto &imag_data( void ) { return imag; }
	const auto &imag_data( void ) const { return imag; }
	
	bool operator==( const number &other ) const {
		return (real == other.real && imag == other.imag);
	}
	bool operator!=( const number &other ) const {
		return !(*this == other);
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator+=( Number &&other ) {
		real += std::forward<Number>( other ).real;
		imag += std::forward<Number>( other ).imag;
		return *this;
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator+( Number &&other ) const {
		return (number{ *this } += std::forward<Number>( other ));
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator-=( Number &&other ) {
		real -= std::forward<Number>( other ).real;
		imag -= std::forward<Number>( other ).imag;
		return *this;
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator-( Number &&other ) const {
		return (number{ *this } -= std::forward<Number>( other ));
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator*=( Number &&other ) {
		auto new_real = real * other.real - imag * other.imag;
		imag *= std::forward<Number>( other ).real;
		real *= std::forward<Number>( other ).imag;
		
		imag += std::move( real );
		real = std::move( new_real );
		
		return *this;
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator*( Number &&other ) const {
		return (number{ *this } *= std::forward<Number>( other ));
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator/=( Number &&other ) {
		auto modulus_squared = real * other.real + imag * other.imag;
		
		real /= modulus_squared;
		imag /= - std::move( modulus_squared );
		
		return *this;
	}
	
	template<class Number, CXXMATH_ENABLE_IF_TAG_IS(Number,number_tag)>
	number &operator/( Number &&other ) const {
		return (number{ *this } /= std::forward<Number>( other ));
	}
	
	number &operator-( void ) && {
		real.backend().negate();
		imag.backend().negate();
		return *this;
	}
	
	number &operator-( void ) const & {
		return - number{ *this };
	}
	
	bool is_real_integer( void ) const {
		return (
			imag.is_zero() &&
			(boost::multiprecision::denominator( real ) == 1)
		);
	}
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const number &num ) {
	return os << "(" << num.real_data() << ", " << num.imag_data() << ")";
}

namespace model_number {
struct equal {
	template<class N1, class N2>
	static constexpr decltype( auto ) apply( N1 &&n1, N2 &&n2 ) {
		return std::forward<N1>( n1 ) == std::forward<N2>( n2 );
	}
};

struct add_assign {
	template<class N1, class N2>
	static constexpr decltype( auto ) apply( N1 &&n1, N2 &&n2 ) {
		return std::forward<N1>( n1 ) += std::forward<N2>( n2 );
	}
};

struct negate_in_place {
	template<class N>
	static constexpr decltype( auto ) apply( N &&n ) {
		return - std::forward<N>( n );
	}
};

struct zero {
	static auto apply( void ) {
		return number{0};
	}
};

struct one {
	static auto apply( void ) {
		return number{1};
	}
};

struct multiply_assign {
	template<class N1, class N2>
	static constexpr decltype( auto ) apply( N1 &&n1, N2 &&n2 ) {
		return std::forward<N1>( n1 ) *= std::forward<N2>( n2 );
	}
};
}
}

namespace cxxmath::impl {
template<> struct tag_of<PQuantum::mathutils::number> {
	using type = PQuantum::mathutils::number_tag;
};

template<> struct make<PQuantum::mathutils::number_tag> {
	template<class RealPart, class ImaginaryPart = int>
	static auto apply( RealPart &&rp, ImaginaryPart && ip = 0 ) {
		return PQuantum::mathutils::number{
			std::forward<RealPart>( rp ),
			std::forward<ImaginaryPart>( ip )
		};
	}
};

template<> struct default_comparable<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::comparable<
		PQuantum::mathutils::model_number::equal
	>;
};

template<> struct default_monoid<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::assignable_monoid<
		PQuantum::mathutils::model_number::multiply_assign,
		PQuantum::mathutils::model_number::one,
		impl::true_
	>;
};

template<> struct default_group<PQuantum::mathutils::number_tag> {
	using type = cxxmath::concepts::assignable_group<
		cxxmath::concepts::assignable_monoid<
			PQuantum::mathutils::model_number::multiply_assign,
			PQuantum::mathutils::model_number::zero,
			impl::true_
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

#endif //PQUANTUM_NUMBER_HPP
