//
// Created by jayz on 09.04.20.
//

#include "lagrangian_manipulations.hpp"

namespace {
using namespace PQuantum;

struct zero_fields_impl {
	const std::vector<support::uuid> &field_ids;
	
	template<
		class Atom,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, model::indexed_field>>
	> model::lagrangian_tree operator()( Atom &&atom ) const {
		return std::forward<Atom>( atom );
	}
	
	model::lagrangian_tree operator()( const model::indexed_field &ifield ) const {
		if( std::find( std::begin( field_ids ), std::end( field_ids ), ifield.id ) != std::end( field_ids ) )
			return mathutils::number{ 0, 0 };
		return ifield;
	}
	
	template<class NonTerminalNodeData, class Children, class TransformedChildren>
	model::lagrangian_tree operator()( NonTerminalNodeData &&nt, Children &&, TransformedChildren &&tch ) const {
		return {
			std::forward<NonTerminalNodeData>( nt ),
			std::forward<TransformedChildren>( tch )
		};
	}
};
}

namespace PQuantum::calculation {
model::lagrangian_tree zero_fields( const model::lagrangian_tree &expr, const std::vector<support::uuid> &field_ids ) {
	return simplify_arithmetic( cxxmath::recursive_tree_transform( expr, zero_fields_impl{ field_ids } ) );
}
}
