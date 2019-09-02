//
// Created by jayz on 31.08.19.
//

#ifndef PQUANTUM_ABSTRACT_INDEX_NOTATION_HPP
#define PQUANTUM_ABSTRACT_INDEX_NOTATION_HPP

#include "mathutils/abstract_algebra/free_polynomial.hpp"

#include "template_helpers/iterator.hpp"

namespace PQuantum
{
namespace mathutils
{
template<class UnderlyingPolynomial, class IndexHandler>
class abstract_index_quotient
{
	static constexpr index_handler = IndexHandler{};
// FIXME: Due to the gcc bug #91609, these things have to be public...
public:
	using abstract_index_polynomial;
	using underlying_polynomial = UnderlyingPolynomial;
	using coefficient_ring = typename underlying_polynomial::coefficient_ring;
	
	template<class SortedIndexSet1, class SortedIndexSet2>
	static auto index_intersection(SortedIndexSet1 &&si1, SortedIndexSet2 &&si2) {
		using template_helpers::begin;
		using template_helpers::end;
		std::vector<std::decay_t<SortedIndexSet1>::value_type> common_indices;
		
		std::set_intersection(begin(c_indices), end(c_indices), begin(v_indices), end(v_indices),
							  std::back_inserter(common_indices),
							  template_helpers::less_to_equal(index_handler.less_indices));
		return common_indices;
	}
	
	template<class Range1, class Range2>
	static auto find_index_coincidence(Range1 &&range1, Range2 &&range2, bool disjoint) {
		for(auto r1_it = std::begin(range1); r1_it !=; ++r1_it) {
			auto r1_indices = index_handler.get_indices(*r1_it);
			std::sort(r1_indices.begin(), r1_indices.end(), index_handler.less_indices);
			
			for(auto r2_it = disjoint ? std::begin(range2) : r1_it + 1; r2_it != std::end(range2); ++r2_it) {
				auto r2_indices = index_handler.get_indices(*r2_it);
				std::sort(r2_indices.begin(), r2_indices.end(), index_handler.less_indices);
				
				auto common_indices = index_intersection(std::move(r1_indices), std::move(r2_indices));
				
				if(std::size(common_indices) != 0)
					return std::make_tuple(r1_it, r2_it, std::move(common_indices));
			}
		}
		
		using indices_type = decltype(index_handler.get_indices(*std::end(range1)));
		using common_indices_type = std::decay_t<decltype(index_intersection(std::declval<indices_type>(),
																			 std::declval<indices_type>()))>;
		return std::make_tuple(std::end(range1), std::end(range2), common_indices_type{});
	}
	
	template<class Function, class CPart, class Variables>
	static underlying_polynomial
	apply_to_cpartv_contractions(Function &&function, CPart &&cpart, Variables &&variables) {
		static_assert(std::is_rvalue_reference_v<CPart>, "cpart is not an rvalue reference.");
		static_assert(std::is_rvalue_reference_v<Variables>, "variables is not an rvalue reference.");
		
		auto index_coincidence = find_index_coincidence(cpart, variables, true);
		auto common_indices = std::get<2>(index_coincidence);
		
		if(std::size(common_indices) == 0)
			return underlying_polynomial{index_handler.compose_coefficient_part(std::move(cpart)),
										 std::move(variables)};
		
		auto c_it = std::get<0>(index_coincidence);
		auto v_it = std::get<1>(index_coincidence);
		
		auto c_pre = boost::make_iterator_range(std::begin(cpart), c_it);
		auto c_post = boost::make_iterator_range(c_it + 1), std::end(cpart));
		
		underlying_polynomial result = function(std::move(c_pre), std::move(*c_it), std::move(c_post), std::move(*v_it),
												std::move(common_indices));
		return (underlying_polynomial{coefficient_ring::one(), {std::make_move_iterator(std::cbegin(variables)),
																std::make_move_iterator(v_it)}} *
				std::move(result) *
				underlying_polynomial{coefficient_ring::one(), {std::make_move_iterator(v_it + 1),
																std::make_move_iterator(std::end(variables))}});
	}
	
	template<class Function, class Coefficient, class Variables>
	static underlying_polynomial
	canonicalize_cv_contractions(Function &&function, Coefficient &&c, Variables &&variables) {
		auto c_decomposed = index_handler.decompose_coefficient(c);
		
		underlying_polynomial result;
		std::for_each(c_decomposed.begin(), c_decomposed.end(), [&variables](auto &&c_part) {
			result += apply_to_cpartv_contractions(std::forward<Function>(f), std::move(c_part), variables);
			);
		}
		
		template<class Function, class Coefficient, class Variables>
		static underlying_polynomial
		apply_to_vv_contractions(Function &&function, Coefficient &&c, Variables &&variables) {
			static_assert(std::is_rvalue_reference_v<Coefficient>, "c is not an rvalue reference.");
			static_assert(std::is_rvalue_reference_v<Variables>, "variables is not an rvalue reference.");
			
			auto index_coincidence = find_index_coincidence(cpart, variables, true);
			auto common_indices = std::get<2>(index_coincidence);
			
			if(std::size(common_indices) == 0)
				return underlying_polynomial{std::move(c), std::move(variables)};
			
			auto v1_it = std::get<0>(index_coincidence);
			auto v2_it = std::get<1>(index_coincidence);
			
			underlying_polynomial result = index_handler.function(v1, boost::make_iterator_range(v1_it + 1, v2_it), v2,
																  std::move(common_indices));
			return (underlying_polynomial{std::move(c), {std::make_move_iterator(std::begin(variables)),
														 std::make_move_iterator(v1_it)}} * std::move(result) *
					underlying_polynomial{coefficient_ring::one(), {std::make_move_iterator(v1_it + 1),
																	std::make_move_iterator(std::end(variables))}});
		}
		
		void canonicalize(abstract_index_polynomial &p) {
			underlying_polynomial contracted = std::move(p);
			std::for_each(p.monomials.begin(), p.monomials.end(), [](auto &&monomial) {
				contracted += apply_to_vv_contractions(index_handler.contract, std::move(monomial.coefficient),
													   std::move(monomial.variables));
			});
			
			p = ring<abstract_index_polynomial>::zero();
			std::for_each(contracted.monomials.begin(), contracted.monomials.end(), [](auto &&monomial) {
				p += apply_to_cv_contractions(index_handler.contract, std::move(monomial.coefficient),
											  std::move(monomial.variables));
			});
		}
		
		static underlying_polynomial expand_all_summations(const abstract_index_polynomial &p) {
			underlying_polynomial expanded = std::move(p);
			std::for_each(p.monomials.begin(), p.monomials.end(), [](auto &&monomial) {
				expanded += apply_to_vv_contractions(index_handler.expand, std::move(monomial.coefficient),
													 std::move(monomial.variables));
			});
			
			p = ring<abstract_index_polynomial>::zero();
			std::for_each(expanded.monomials.begin(), expanded.monomials.end(), [](auto &&monomial) {
				p += apply_to_cv_contractions(index_handler.expand, std::move(monomial.coefficient),
											  std::move(monomial.variables));
			});
		}
		
		static bool equal(const abstract_index_polynomial &p1, const abstract_index_polynomial &p2) {
			return (expand_all_summations(p1) == expand_all_summations(p2);
		}
		
		public:
		using type = abstract_index_polynomial;
	};
}
}

#endif //PQUANTUM_ABSTRACT_INDEX_NOTATION_HPP
