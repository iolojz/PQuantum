//
// Created by jayz on 02.04.20.
//

#include "lagrangian_manipulations.hpp"

#include <boost/range/combine.hpp>

namespace {
using namespace PQuantum;

class derive_lagrangian_impl {
	const std::map<support::uuid, support::uuid> &delta_field_map;
public:
	derive_lagrangian_impl( const std::map<support::uuid, support::uuid> &dfm ) : delta_field_map{ dfm } {}
	
	template<class Atom> model::lagrangian_tree operator()( const Atom & ) const {
		return mathutils::number{ 0, 0 };
	}
	
	template<class Summands, class TransformedSummands>
	model::lagrangian_tree operator()( mathutils::sum, Summands &&, TransformedSummands &&ts ) const {
		return { mathutils::sum{}, std::forward<TransformedSummands>( ts ) };
	}
	
	template<class Args, class TransformedArgs>
	delta_lagrangian_tree operator()( mathutils::difference, Args &&, TransformedArgs &&ts ) const {
		return { mathutils::difference{}, std::forward<TransformedArgs>( ts ) };
	}
	
	template<class Factors, class TransformedFactors>
	model::lagrangian_tree operator()( mathutils::product, Factors &&f, TransformedFactors &&tf ) const {
		using product_node = std::decay_t<
			decltype(cxxmath::get_node<mathutils::product>( std::declval<delta_lagrangian_tree>() ))
		>;
		std::vector<delta_lagrangian_tree> summands;
		
		auto tf_it = std::begin( tf );
		for( std::size_t i = 0; i != std::size( tf ); ++i ) {
			product_node term = { mathutils::product{}, f };
			term.children[i] = *tf_it++;
			summands.emplace_back( std::move( term ) );
		}
		
		return {
			mathutils::sum{},
			std::move( summands )
		};
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree operator()( mathutils::quotient, Args &&a, TransformedArgs &&ta ) const {
		if( cxxmath::holds_node<mathutils::number>( ta.back() ) ) {
			if( cxxmath::get_node<mathutils::number>( ta.back() ).data == mathutils::number{ 0, 0 } )
				return { mathutils::quotient{}, ta.front(), a.back() };
		}
		
		return {
			mathutils::difference{},
			model::lagrangian_tree{
				mathutils::quotient{},
				ta.front(),
				a.back()
			},
			model::lagrangian_tree{
				mathutils::quotient{},
				model::lagrangian_tree{
					mathutils::product{},
					a.front(),
					ta.back()
				},
				model::lagrangian_tree{
					mathutils::product{},
					a.back(),
					a.back()
				}
			},
		};
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree operator()( mathutils::power, Args &&a, TransformedArgs &&ta ) const {
		if( cxxmath::holds_node<mathutils::number>( a.back() ) ) {
			if( cxxmath::get_node<mathutils::number>( a.back() ).data == mathutils::number{ 0, 0 } )
				return mathutils::number{ 0, 0 };
			if( cxxmath::get_node<mathutils::number>( a.back() ).data == mathutils::number{ 1, 0 } )
				return ta.front();
		}
		
		return {
			mathutils::sum{},
			model::lagrangian_tree{
				mathutils::product{},
				model::lagrangian_tree{
					mathutils::power{},
					a.front(),
					model::lagrangian_tree{
						mathutils::difference{},
						a.back(),
						mathutils::number{ 1, 0 }
					}
				},
				a.back(),
				ta.front()
			},
			model::lagrangian_tree{
				mathutils::product{},
				delta_lagrangian_tree{
					mathutils::function_call<std::string>{ "ln" },
					a.front()
				},
				model::lagrangian_tree{
					mathutils::power{},
					a.front(),
					a.back()
				},
				ta.back()
			}
		};
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree
	operator()( const mathutils::function_call<std::string> &f, Args &&a, TransformedArgs &&ta ) const {
		if( f.atom == "ln" ) {
			return {
				mathutils::quotient{},
				std::move( *std::begin( ta ) ),
				std::move( *std::begin( a ) )
			};
		}
		
		throw std::runtime_error( "Cannot take derivative of given function_call" );
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree operator()( mathutils::negation, Args &&, TransformedArgs &&ta ) const {
		return { mathutils::negation{}, std::forward<TransformedArgs>( ta ) };
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree operator()( const mathutils::spacetime_derivative &sd, Args &&, TransformedArgs &&ta ) const {
		return { sd, std::forward<TransformedArgs>( ta ) };
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree operator()( mathutils::dirac_operator, Args &&, TransformedArgs &&ta ) const {
		return { mathutils::dirac_operator{}, std::forward<TransformedArgs>( ta ) };
	}
	
	template<class Args, class TransformedArgs>
	model::lagrangian_tree operator()( mathutils::field_multiplication_operator, Args &&, TransformedArgs &&ta ) const {
		auto &&transformed = *std::begin( ta );
		if( !(cxxmath::holds_node<model::indexed_field>( transformed ) ||
		    cxxmath::holds_node<delta_indexed_field>( transformed )) )
			return mathutils::number{ 0, 0 };
		
		return { mathutils::field_multiplication_operator{}, std::move( transformed ) };
	}
};

template<>
model::lagrangian_tree
derive_lagrangian_impl::operator()<model::indexed_field>( const model::indexed_field &ifield ) const {
	auto delta_it = delta_field_map.find( ifield.id );
	if( delta_it == delta_field_map.end() )
		return mathutils::number{ 0, 0 };
	
	return model::indexed_field{ delta_it->second, ifield.indices };
}

model::lagrangian_tree take_derivative(
	const std::map<support::uuid, support::uuid> &delta_field_map,
	const model::lagrangian_tree &lagrangian
) {
	return cxxmath::recursive_tree_transform( delta_lagrangian, derive_lagrangian_t{delta_field_map} );
}
}