//
// Created by jayz on 02.04.20.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_DERIVATIVE_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_DERIVATIVE_HPP

#include "lagrangian.hpp"

namespace PQuantum::model {
class derive_lagrangian_t {
	int delta_index;
public:
	derive_lagrangian_t( int di ) : delta_index{ di } {}
	
	template<class Atom> delta_lagrangian_tree operator()( const Atom &atom ) const {
		return mathutils::number{ 0, 0 };
	}
	
	template<class Summands, class TransformedSummands>
	delta_lagrangian_tree operator()( mathutils::sum, Summands &&, TransformedSummands &&ts ) const {
		return { mathutils::sum{}, std::forward<TransformedSummands>( ts ) };
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( mathutils::difference, Args &&, TransformedArgs &&ts ) const {
		return { mathutils::difference{}, std::forward<TransformedArgs>( ts ) };
	}
	
	template<class Factors, class TransformedFactors>
	delta_lagrangian_tree operator()( mathutils::product, Factors &&f, TransformedFactors &&fs ) const {
	
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( mathutils::quotient, Args &&a, TransformedArgs &&ta ) const {
		return delta_lagrangian_tree{
			mathutils::difference{},
			delta_lagrangian_tree{
				mathutils::quotient{},
				ta.front(),
				a.back()
			},
			delta_lagrangian_tree{
				mathutils::quotient{},
				delta_lagrangian_tree{
					mathutils::product{},
					a.front(),
					ta.back()
				},
				delta_lagrangian_tree{
					mathutils::product{},
					a.back(),
					a.back()
				}
			},
		};
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( mathutils::power, Args &&a, TransformedArgs &&ta ) const {
		return delta_lagrangian_tree{
			mathutils::sum{},
			delta_lagrangian_tree{
				mathutils::product{},
				delta_lagrangian_tree{
					mathutils::power{},
					a.front(),
					delta_lagrangian_tree{
						mathutils::difference{},
						a.back(),
						mathutils::number{ -1, 0 }
					}
				},
				ta.front()
			},
			delta_lagrangian_tree{
				mathutils::product{},
				delta_lagrangian_tree{
					mathutils::function_call{ "ln" },
					a.front()
				},
				delta_lagrangian_tree{
					mathutils::power{},
					a.front(),
					a.back()
				},
				ta.back()
			}
		};
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( const mathutils::function_call &f, Args &&a, TransformedArgs &&ta ) const {
		if( f.atom == "ln" ) {
			delta_lagrangian_tree{
				mathutils::quotient{},
				ta,
				a
			};
		}
		
		throw std::runtime_error( "Cannot take derivative of given function_call" );
		return {};
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( mathutils::negation, Args &&, TransformedArgs &&ta ) const {
		return { mathutils::negation{}, std::forward<TransformedArgs>( ta ) };
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( mathutils::parentheses, Args &&, TransformedArgs &&ta ) const {
		return { mathutils::parentheses{}, std::forward<TransformedArgs>( ta ) };
	}
};

template<>
delta_lagrangian_tree derive_lagrangian_t::operator()<indexed_field>( const indexed_field &ifield ) const {
	return delta_indexed_field{ ifield, delta_index };
}
template<>
delta_lagrangian_tree derive_lagrangian_t::operator()<delta_indexed_field>( const delta_indexed_field &difield ) const {
	return difield;
}

static delta_lagrangian_tree take_nth_derivative( int n, const lagrangian_tree &lagrangian ) {
	return cxxmath::recursive_tree_transform( lagrangian, derive_lagrangian_t{n} );
}

static delta_lagrangian_tree take_nth_derivative( int n, const delta_lagrangian_tree &delta_lagrangian ) {
	return cxxmath::recursive_tree_transform( delta_lagrangian, derive_lagrangian_t{n} );
}
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_DERIVATIVE_HPP
