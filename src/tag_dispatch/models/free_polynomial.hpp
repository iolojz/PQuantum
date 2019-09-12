//
// Created by jayz on 15.08.19.
//

#ifndef TAGD_MODELS_FREE_POLYNOMIAL_HPP
#define TAGD_MODELS_FREE_POLYNOMIAL_HPP

#include "free_monomial.hpp"
#include "tag_dispatch/erase_if.hpp"
#include "tag_dispatch/operator_helpers.hpp"

#include <boost/container/flat_map.hpp>
#include <boost/range/combine.hpp>

namespace tag_dispatch {
namespace models {
template<class Coefficient, class Variable>
struct free_polynomial_tag {
	using coefficient = Coefficient;
	using variable = Variable;
};

template<class Coefficient, class Variable, class CoefficientRing, class TotalVariablePreorder>
class free_polynomial {
public:
	using dispatch_tag = free_polynomial_tag<Coefficient, Variable>;
	
	using coefficient = Coefficient;
	using coefficient_ring = CoefficientRing;
	using variable = Variable;
	
	using monomial = free_monomial<coefficient, variable>;
private:
	using coefficient_tag = tag_of_t<coefficient>;
	using add_assign_coefficients = impl::add_assign<coefficient_tag, coefficient_ring>;
	using equal_coefficients = impl::add_assign<coefficient_tag, coefficient_ring>;
	using zero_coefficient = impl::zero<coefficient_tag, coefficient_ring>;
	using negate_coefficient_in_place = impl::negate_in_place<coefficient_tag, coefficient_ring>;
	
	struct less_variables {
		template<class V1, class V2>
		decltype(auto) operator()(V1 &&v1, V2 &&v2) const {
			return impl::less_equal<tag_of_t<Variable>, TotalVariablePreorder>::apply(std::forward<V1>(v1),
																					  std::forward<V2>(v2));
		}
	};
	
	struct less_variable_ranges {
		template<class Range1, class Range2>
		bool operator()(Range1 &&r1, Range2 &&r2) const {
			return std::lexicographical_compare(std::begin(r1), std::end(r1), std::begin(r2), std::end(r2),
												less_variables{});
		}
	};
	
	void strip_zeros(void) {
		erase_if(std::begin(monomial_map), std::end(monomial_map), [](const auto &map_entry) {
			return equal_coefficients::apply(std::get<0>(map_entry), zero_coefficient::apply());
		});
	}
	
	// FIXME: Should we enable runtime switching of this type?
	using monomial_container = boost::container::flat_map<decltype(std::declval<monomial>().variables), coefficient, less_variable_ranges>;
	monomial_container monomial_map;
public:
	free_polynomial(const free_polynomial &) = default;
	
	free_polynomial(free_polynomial &&) = default;
	
	explicit free_polynomial(const monomial &m) : monomial_map{{m.variables, m.coefficient}} {}
	
	explicit free_polynomial(monomial &&m) : monomial_map{{std::move(m.variables), std::move(m.coefficient)}} {}
	
	const monomial_container &monomials(void) const { return monomial_map; }
	
	free_polynomial &operator+=(const free_polynomial &p) {
		if(this == &p) {
			for(auto &term : p.monomials())
				add_assign_coefficients::apply(term.second, term.second);
			
			return *this;
		}
		
		for(const auto &term : p.monomials()) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				add_assign_coefficients::apply(insertion_result.first->second, term.second);
				if(equal_coefficients::apply(insertion_result.first->second, zero_coefficient::apply()))
					monomial_map.erase(insertion_result.first);
			}
		}
		
		return *this;
	}
	
	free_polynomial &operator+=(free_polynomial &&p) {
		if(this == &p) {
			for(auto &term : p.monomials())
				add_assign_coefficients::apply(term.second, term.second);
			return *this;
		}
		
		for(auto &&term : p.monomials()) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				add_assign_coefficients::apply(insertion_result.first->second, std::move(term.second));
				if(equal_coefficients::apply(insertion_result.first->second, zero_coefficient::apply()))
					monomial_map.erase(insertion_result.first);
			}
		}
		
		return *this;
	}
	
	template<class Polynomial>
	free_polynomial &operator-=(Polynomial &&p) {
		return *this += -std::forward<Polynomial>(p);
	}
	
	TAGD_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial, +)
	
	TAGD_REVERSIBLE_BINARY_OPERATOR_OVERLOAD(free_polynomial, -, -)
	
	free_polynomial operator-(void) &&{
		for(auto &term : monomial_map)
			term.second = negate_coefficient_in_place::apply(std::move(term.second));
		
		return std::move(*this);
	}
	
	free_polynomial operator-(void) const &{ return -free_polynomial{*this}; }
	
	free_polynomial &operator*=(const free_polynomial &p) {
		std::vector<monomial> monomials;
		for(const auto &factor1 : monomial_map) {
			for(const auto &factor2 : p.monomial_map) {
				auto concatenated_variables = factor1.first;
				concatenated_variables.insert(concatenated_variables.end(), factor2.first.begin(), factor2.first.end());
				
				monomials.push_back({std::move(concatenated_variables), factor1.second * factor2.second});
			}
		}
		
		decltype(monomial_map) result;
		for(auto &&monomial : monomials) {
			if(coefficient_ring::equal(monomial.coeff, coefficient_ring::zero()))
				continue;
			
			auto insertion_result = result.insert({monomial.variables, monomial.coeff});
			if(insertion_result.second == false) {
				coefficient_ring::add_assign(insertion_result.first->second, std::move(monomial.coeff));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					result.erase(insertion_result.first);
			}
		}
		
		monomial_map = std::move(result);
		return *this;
	}
	
	TAGD_BINARY_OPERATOR_OVERLOAD(free_polynomial, *)
	
	bool operator==(const free_polynomial &p) const {
		// There is no documentation on how boost::container::flat_map actually implements the comparisons...
		if(monomial_map.size() != p.monomial_map.size())
			return false;
		
		for(const auto &map_entries : boost::combine(monomial_map, p.monomial_map)) {
			const auto &variables1 = boost::get<0>(map_entries).first;
			const auto &variables2 = boost::get<1>(map_entries).first;
			
			if(variables1.size() != variables2.size())
				return false;
			
			for(const auto &variables : boost::combine(variables1, variables2)) {
				using not_equal = impl::not_equal<tag_of_t<Variable>, quotient_tag<tag_of_t<Variable>, TotalVariablePreorder>>;
				if(not_equal::apply(boost::get<0>(variables), boost::get<1>(variables)))
					return false;
			}
			
			if(!coefficient_ring::equal(boost::get<0>(map_entries).second, boost::get<1>(map_entries).second))
				return false;
		}
		
		return true;
	}
	
	bool operator!=(const free_polynomial &p) const { return !(*this == p); }
};
}

namespace impl {
template<class DispatchTag, class StructureTag>
struct internal_total_preorder;
template<class DispatchTag, class StructureTag> using internal_total_preorder_t = internal_total_preorder<DispatchTag, StructureTag>;

template<class DispatchTag, class StructureTag>
struct underlying_polynomial;
template<class DispatchTag, class StructureTag> using underlying_polynomial_t = underlying_polynomial<DispatchTag, StructureTag>;

template<class Coefficient, class Variable, class StructureTag>
struct underlying_ring<models::free_polynomial_tag<Coefficient, Variable>, StructureTag> {
	using type = concepts::ring<tag_of_t<Coefficient>, StructureTag>;
	static_assert(type::value, "Underlying ring does not model the 'ring' concept.");
};

template<class Coefficient, class Variable, class StructureTag>
struct underlying_polynomial<models::free_polynomial_tag<Coefficient, Variable>, StructureTag> {
private:
	using coefficient_ring = underlying_ring_t<models::free_polynomial_tag<Coefficient, Variable>, StructureTag>;
	using preorder = internal_total_preorder_t<models::free_polynomial_tag<Coefficient, Variable>, StructureTag>;
public:
	using type = models::free_polynomial<Coefficient, Variable, coefficient_ring, preorder>;
};

template<class Coefficient, class Variable>
struct internal_total_preorder<models::free_polynomial_tag<Coefficient, Variable>, concepts::ring<models::free_polynomial_tag<Coefficient, Variable>, void> > {
	using type = std::decay_t<decltype(default_structure_tag<tag_of_t<Variable>>(impl_tag<less_equal>{}))>;
};

template<class Coefficient, class Variable>
struct equal<models::free_polynomial_tag<Coefficient, Variable>, concepts::set<models::free_polynomial_tag<Coefficient, Variable>, void>> {
	template<class P1, class P2>
	static decltype(auto) apply(P1 &&p1, P2 &&p2) {
		return std::forward<P1>(p1) == std::forward<P2>(p2);
	}
};

template<class Coefficient, class Variable, class StructureTag>
struct make<models::free_polynomial_tag<Coefficient, Variable>, concepts::makeable<models::free_polynomial_tag<Coefficient, Variable>, StructureTag>> {
private:
	using tag = models::free_polynomial_tag<Coefficient, Variable>;
	using polynomial_ring = concepts::ring<models::free_polynomial_tag<Coefficient, Variable>, StructureTag>;
	using polynomial = underlying_polynomial<tag, polynomial_ring>;
	
	static constexpr void add_to(polynomial &) {}
	
	template<class FirstMonomial, class ...Monomials>
	static void add_to(polynomial &p, FirstMonomial &&m, Monomials &&...monomials) {
		p += polynomial{std::forward<FirstMonomial>(m)};
		add_to(p, std::forward<Monomials>(monomials)...);
	}

public:
	static polynomial apply(const polynomial &p) { return p; }
	
	static polynomial apply(polynomial &&p) { return std::move(p); }
	
	template<class ...Monomials>
	static polynomial apply(Monomials &&...monomials) {
		polynomial result = zero<tag, polynomial_ring>();
		add_to(result, std::forward<Monomials>(monomials)...);
		return result;
	}
};

template<class Coefficient, class Variable>
struct is_abelian<models::free_polynomial_tag<Coefficient, Variable>, concepts::monoid<models::free_polynomial_tag<Coefficient, Variable>, void>> {
	static constexpr bool apply(void) { return true; }
};

template<class Coefficient, class Variable>
struct compose_assign<models::free_polynomial_tag<Coefficient, Variable>, concepts::monoid<models::free_polynomial_tag<Coefficient, Variable>, void>> {
	template<class P1, class P2>
	static decltype(auto) apply(P1 &&p1, P2 &&p2) {
		return std::forward<P1>(p1) += std::forward<P2>(p2);
	}
};

template<class Coefficient, class Variable>
struct neutral_element<models::free_polynomial_tag<Coefficient, Variable>, concepts::monoid<models::free_polynomial_tag<Coefficient, Variable>, void>> {
private:
	using polynomial_ring = concepts::ring<models::free_polynomial_tag<Coefficient, Variable>>;
public:
	static underlying_polynomial_t<models::free_polynomial_tag<Coefficient, Variable>, polynomial_ring> apply(void) {
		return {};
	}
};

template<class Coefficient, class Variable>
struct inverse_in_place<models::free_polynomial_tag<Coefficient, Variable>, concepts::group<models::free_polynomial_tag<Coefficient, Variable>, void>> {
	template<class P>
	static decltype(auto) apply(P &&p) { return -std::forward<P>(p); }
};

template<class Coefficient, class Variable>
struct is_abelian<models::free_polynomial_tag<Coefficient, Variable>, concepts::ring<models::free_polynomial_tag<Coefficient, Variable>, void>> {
	static constexpr bool apply(void) { return false; }
};

template<class Coefficient, class Variable>
struct multiply_assign<models::free_polynomial_tag<Coefficient, Variable>, concepts::ring<models::free_polynomial_tag<Coefficient, Variable>, void>> {
	template<class P1, class P2>
	static decltype(auto) apply(P1 &&p1, P2 &&p2) {
		return std::forward<P1>(p1) *= std::forward<P2>(p2);
	}
};

template<class Coefficient, class Variable>
struct one<models::free_polynomial_tag<Coefficient, Variable>, concepts::ring<models::free_polynomial_tag<Coefficient, Variable>, void>> {
private:
	using polynomial_ring = concepts::ring<models::free_polynomial_tag<Coefficient, Variable>>;
	using polynomial = underlying_polynomial_t<models::free_polynomial_tag<Coefficient, Variable>, polynomial_ring>;
	using coefficient_ring = typename polynomial::coefficient_ring;
	using monomial = typename polynomial::monomial;
public:
	static auto apply(void) {
		return polynomial{monomial{one<tag_of_t<Coefficient>, coefficient_ring>::apply(), {}}};
	}
};
}
}

/*
template<class Coefficient, class Variable, class LessVariables>
class free_polynomial {
public:
	using dispatch_tag = free_polynomial_tag<Coefficient, Variable>;
	
	using coefficient = Coefficient;
	using variable = Variable;
	using less_variables = LessVariables;
	
	using monomial = models::monomial<coefficient, variable>;
private:
	struct less_variable_ranges {
		template<class Range1, class Range2>
		bool operator()(Range1 &&r1, Range2 &&r2) const {
			return std::lexicographical_compare(std::begin(r1), std::end(r1), std::begin(r2), std::end(r2),
												less_variables{});
		}
	};
	
	// FIXME: Should we enable runtime switching of this type?
	using monomial_container = boost::container::flat_map<decltype(std::declval<monomial>().variables), coefficient, less_variable_ranges>;
	monomial_container monomial_map;
	
	void strip_zeros(void) {
		template_helpers::erase_if(monomial_map, [](auto &&map_entry) {
			return coefficient_ring::equal(map_entry.second, coefficient_ring::zero());
		});
	}
public:
	free_polynomial(void) = default;
	
	template<class Monomial, PQUANTUM_ENABLE_IF_TAG_IS(Monomial, monomial_tag)>
	free_polynomial(Monomial &&m) : monomial_map{std::forward<Monomial>(m)} { strip_zeros(); }
	
	template<class MonomialRange>
	free_polynomial(MonomialRange &&range)
			: monomial_map{std::begin(range), std::end(range)} { strip_zeros(); }
	
	free_polynomial &operator+=(const free_polynomial &p) {
		if(&p == this) {
			for(auto &term : monomial_map)
				coefficient_ring::add_assign(term.second, term.second);
			
			return *this;
		}
		
		for(const auto &term : p.terms) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::add_assign(insertion_result.first->second, term.second);
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			}
		}
		
		return *this;
	}
	
	free_polynomial &operator+=(free_polynomial &&p) {
		if(&p == this) {
			for(auto &term : monomial_map)
				coefficient_ring::add_assign(term.second, term.second);
			
			return *this;
		}
		
		for(auto &&term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::add_assign(insertion_result.first->second, std::move(term.second));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			}
		}
		
		return *this;
	}
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial,
	+)
	
	free_polynomial &operator-=(const free_polynomial &p) {
		if(&p == this) {
			monomial_map.clear();
			return *this;
		}
		
		for(const auto &term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::subtract_assign(insertion_result.first->second, term.second);
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			} else
				insertion_result.first->second = -insertion_result.first->second;
		}
		
		return *this;
	}
	
	free_polynomial &operator-=(free_polynomial &&p) {
		if(&p == this) {
			monomial_map.clear();
			return *this;
		}
		
		for(auto &&term : p.monomial_map) {
			auto insertion_result = monomial_map.insert(term);
			if(insertion_result.second == false) {
				coefficient_ring::subtract_assign(insertion_result.first->second, std::move(term.second));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					monomial_map.erase(insertion_result.first);
			} else
				insertion_result.first->second = -insertion_result.first->second;
		}
		
		return *this;
	}
	
	PQUANTUM_BINARY_OPERATOR_OVERLOAD(free_polynomial,
	-, -)
	
	free_polynomial operator-(void) &&{
		for(auto &term : monomial_map)
			term.second = coefficient_ring::negate(std::move(term.second));
		
		return std::move(*this);
	}
	
	free_polynomial operator-(void) const &{ return -free_polynomial{*this}; }
	
	free_polynomial &operator*=(const free_polynomial &p) {
		std::vector<monomial> monomials;
		for(const auto &factor1 : monomial_map) {
			for(const auto &factor2 : p.monomial_map) {
				auto concatenated_variables = factor1.first;
				concatenated_variables.insert(concatenated_variables.end(), factor2.first.begin(), factor2.first.end());
				
				monomials.push_back({std::move(concatenated_variables), factor1.second * factor2.second});
			}
		}
		
		decltype(monomial_map) result;
		for(auto &&monomial : monomials) {
			if(coefficient_ring::equal(monomial.coeff, coefficient_ring::zero()))
				continue;
			
			auto insertion_result = result.insert({monomial.variables, monomial.coeff});
			if(insertion_result.second == false) {
				coefficient_ring::add_assign(insertion_result.first->second, std::move(monomial.coeff));
				if(coefficient_ring::equal(insertion_result.first->second, coefficient_ring::zero()))
					result.erase(insertion_result.first);
			}
		}
		
		monomial_map = std::move(result);
		return *this;
	}
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial, *
	)
	
	bool operator==(const free_polynomial &p) const {
		if(monomial_map.size() != p.monomial_map.size())
			return false;
		
		for(const auto &map_entries : boost::combine(monomial_map, p.monomial_map)) {
			const auto &variables1 = boost::get<0>(map_entries).first;
			const auto &variables2 = boost::get<1>(map_entries).first;
			
			if(variables1.size() != variables2.size())
				return false;
			
			for(const auto &variables : boost::combine(variables1, variables2)) {
				if(!template_helpers::equal(boost::get<0>(variables), boost::get<1>(variables), less_variables{}))
					return false;
			}
			
			if(!coefficient_ring::equal(boost::get<0>(map_entries).second, boost::get<1>(map_entries).second))
				return false;
		}
		
		return true;
	}
	
	bool operator!=(const free_polynomial &p) const { return !(*this == p); }
	
	auto monomials(void) const { return monomial_map; }
};

template<class FreePolynomial, PQUANTUM_ENABLE_IF_TAG_IS(FreePolynomial, free_polynomial_tag)>
static std::ostream &operator<<(std::ostream &os, const FreePolynomial &p) {
	for(const auto &map_entry : p.monomials()) {
		os << map_entry.second;
		for(const auto &v : map_entry.first)
			os << " " << v;
	}
	
	return os;
}
}

namespace impl {
template<class Coefficient, class Variable>
struct set_impl<free_polynomial_tag>
{
	template<class Polynomial1, class Polynomial2>
	static bool equal( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return ( std::forward<Polynomial1>( p1 ) == std::forward<Polynomial2>( p2 ));
	}
};

template<class Coefficient, class Variable, class LessVariables, class CoefficientRingTag>
struct group_impl<free_polynomial<Coefficient, Variable, LessVariables, CoefficientRingTag>>
{
private:
	using polynomial = free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>;
public:
	static constexpr bool is_abelian = true;
	
	template<class Polynomial1, class Polynomial2>
	static constexpr decltype( auto ) compose_assign( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return ( std::forward<Polynomial1>( p1 ) += std::forward<Polynomial2>( p2 ));
	}
	
	template<class Polynomial>
	static constexpr decltype(auto) inverse(Polynomial &&p) { return -std::forward<Polynomial>(p); }
	
	static constexpr decltype(auto) neutral_element() { return polynomial{}; }
};

template<class Variable, class Coefficient, class LessVariables, class CoefficientRingTag>
struct ring_impl<free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>>
{
private:
	using polynomial = free_polynomial<Variable, Coefficient, LessVariables, CoefficientRingTag>;
public:
	template<class Polynomial1, class Polynomial2>
	static decltype( auto ) multiply_assign( Polynomial1 &&p1, Polynomial2 &&p2 )
	{
		return ( std::forward<Polynomial1>( p1 ) *= std::forward<Polynomial2>( p2 ));
	}
	
	static auto one()
	{ return polynomial{ ring<Coefficient, CoefficientRingTag>::one() }; }
};
}

#ifdef PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD
#error "PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD already defined"
#endif

#define PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(op) \
free_polynomial_quotient &operator op ## =( const free_polynomial_quotient &c ) &\
{ \
    *static_cast<base *>( this ) op ## = c; \
    quotient::canonicalize( *this ); \
    return *this; \
} \
\
free_polynomial_quotient operator op ## =( const free_polynomial_quotient &c ) && \
{ \
    std::move( *static_cast<base *>( this ) ) op ## = c; \
    quotient::canonicalize( *this ); \
    return std::move( *this ); \
} \
free_polynomial_quotient &operator op ## =( free_polynomial_quotient &&c ) & \
{ \
    *static_cast<base *>( this ) op ## = std::move( c ); \
    quotient::canonicalize( *this ); \
    return *this; \
} \
\
free_polynomial_quotient operator op ## =( free_polynomial_quotient &&c ) && \
{ \
    std::move( *static_cast<base *>( this ) ) op ## = std::move( c ); \
    quotient::canonicalize( *this ); \
    return std::move( *this ); \
}

template<class QuotientTraits>
class free_polynomial_quotient {
public:
	using quotient_traits = QuotientTraits;
	using underlying_polynomial = typename quotient_traits::underlying_polynomial;
	
	using variable = typename underlying_polynomial::variable;
	using less_variables = typename underlying_polynomial::less_variables;
	using coefficient = typename underlying_polynomial::coefficient;
	using coefficient_ring = typename underlying_polynomial::coefficient_ring;
	
	free_polynomial_quotient(void) = default;
	
	free_polynomial_quotient( const free_polynomial_quotient & ) = default;
	
	free_polynomial_quotient( free_polynomial_quotient && ) = default;
	
	template<class C, class InputIterator>
	free_polynomial_quotient(C &&c, InputIterator begin, InputIterator end)
			: base{std::forward<C>(c), begin, end} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(const base &p) : base{p} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(base &&p) : base{std::move(p)} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(const coefficient &c) : base{c} { quotient::canonicalize(*this); }
	
	explicit free_polynomial_quotient(coefficient &&c) : base{std::move(c)} { quotient::canonicalize(*this); }
	
	PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(+)
	
	PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(-)
	
	PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD(*)
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial_quotient, +)
	
	PQUANTUM_COMMUTATIVE_BINARY_OPERATOR_OVERLOAD(free_polynomial_quotient, *)
	
	PQUANTUM_BINARY_OPERATOR_OVERLOAD(free_polynomial_quotient, -, -)
	
	auto operator-( void ) const &
	{ return free_polynomial_quotient{ -static_cast<const base &>( *this ) }; }
	
	auto operator-( void ) &&
	{ return free_polynomial_quotient{ -std::move( static_cast<base &&>( *this )) }; }
	
	bool operator==( const free_polynomial_quotient &p ) const
	{ return quotient::equal( *this, p ); }
	
	bool operator!=( const free_polynomial_quotient &p ) const
	{ return !( *this == p ); }
	
	using base::monomials;
	
	template<class Var, class Coeff, template<class V, class C, class Less, class CRTag> class Quot, class LessV, class CoeffRTag>
	friend std::ostream &
	operator<<( std::ostream &os, const free_polynomial_quotient<Var, Coeff, Quot, LessV, CoeffRTag> &p );
};

template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag>
static std::ostream &operator<<( std::ostream &os,
								 const free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag> &p )
{
	using polynomial = typename free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>::underlying_polynomial;
	return os << static_cast<const polynomial &>( p );
}

#undef PQUANTUM_QUOTIENT_OPERATORASSIGN_OVERLOAD

namespace abstract_algebra
{
template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag, class Tag>
struct set_impl<free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>, Tag>
{
private:
	using quotient = free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>;
public:
	template<class PQuotient1, class PQuotient2>
	static bool equal( PQuotient1 &&q1, PQuotient2 &&q2 )
	{
		return ( std::forward<PQuotient1>( q1 ) == std::forward<PQuotient2>( q2 ));
	}
};

template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag, class Tag>
struct group_impl<free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>, Tag>
{
private:
	using quotient = free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>;
	using polynomial = typename quotient::underlying_polynomial;
public:
	static constexpr bool is_abelian = group<polynomial, Tag>::is_abelian;
	
	template<class PQuotient1, class PQuotient2>
	static bool compose_assign( PQuotient1 &&q1, PQuotient2 &&q2 )
	{ return ( std::forward<PQuotient1>( q1 ) += std::forward<PQuotient2>( q2 )); }
	
	template<class PQuotient>
	static decltype( auto ) inverse( PQuotient &&q )
	{ return -std::forward<Quotient>( q ); }
	
	static quotient neutral_element( void )
	{ return {}; }
};

template<class Variable, class Coefficient, template<class V, class C, class Less, class CRTag> class Quotient, class LessVariables, class CoefficientRingTag, class Tag>
struct ring_impl<free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>, Tag>
{
private:
	using quotient = free_polynomial_quotient<Variable, Coefficient, Quotient, LessVariables, CoefficientRingTag>;
public:
	template<class PQuotient1, class PQuotient2>
	static bool multiply_assign( PQuotient1 &&q1, PQuotient2 &&q2 )
	{ return ( std::forward<PQuotient1>( q1 ) *= std::forward<PQuotient2>( q2 )); }
	
	static auto one( void )
	{ return quotient{ quotient::coefficient_ring::one() }; }
};
}
}

namespace tag_dispatch {
template<class Coefficient, class Variable, class CoefficientRingTag>
struct tag_of_impl<free_polynomial < Coefficient, Variable, CoefficientRingTag>> {
using type = mathutils::polynomial_tag;
};
}
}
 
 */

#endif //TAGD_MODELS_FREE_POLYNOMIAL_HPP
