//
// Created by jayz on 15.08.19.
//

#ifndef PQUANTUM_NUMBER_HPP
#define PQUANTUM_NUMBER_HPP

#include <utility>

#include "abstract_algebra/ring.hpp"
#include "abstract_algebra/operator_helpers.hpp"

namespace PQuantum
{
namespace mathutils
{
// TODO: implement numbers...
class number
{
public:
	static number one(void) noexcept { return {}; }
	
	constexpr number &operator+=(const number &)
	{ return *this; };
	
	constexpr number &operator-=(const number &) { return *this; };
	
	constexpr number &operator*=(const number &)
	{ return *this; };
	
	constexpr number operator-(void) const { return *this; };
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(number, +)
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(number, *)
	
	PQUANTUM_BINARY_OPERATOR_OVERLOAD(number, -, -)
	
	constexpr bool operator==(const number &) const { return true; }
};

namespace abstract_algebra {
// group of numbers under addition (no overflow detection)
template<>
struct group<number, void> {
	static constexpr bool is_abelian = true;
	
	static constexpr number compose(const number &a, const number &b) noexcept { return a + b; }
	
	static constexpr number inverse(const number &a) noexcept { return -a; }
	
	static constexpr number neutral_element(void) noexcept { return number{}; }
};

// ring of numbers under addition and multiplication (no overflow detection)
template<>
struct ring<number, void> {
	static constexpr number add(const number &a, const number &b) noexcept { return group<number>::compose(a, b); }
	
	static constexpr number multiply(const number &a, const number &b) noexcept { return a * b; }
	
	static constexpr number negate(const number &a) noexcept { return group<number>::inverse(a); }
	
	static constexpr number zero(void) noexcept { return group<number>::neutral_element(); }
	
	static constexpr number one(void) noexcept { return group<number>::neutral_element(); }
};
}
}
}

#endif //PQUANTUM_NUMBER_HPP
