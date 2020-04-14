//
// Created by jayz on 14.04.20.
//

//
// Created by jayz on 09.04.20.
//

#include "lagrangian_manipulations.hpp"

namespace {
using namespace PQuantum;

struct map_fields_impl {
	const std::map<support::uuid, support::uuid> &field_map;
	
	template<
		class Atom,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, model::indexed_field>>
	> model::lagrangian_tree operator()( Atom &&atom ) const {
		return std::forward<Atom>( atom );
	}
	
	model::lagrangian_tree operator()( const model::indexed_field &ifield ) const {
		auto it = field_map.find( ifield.id );
		if( it != std::end( field_map ) )
			return model::indexed_field{ it->second, ifield.indices };
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
model::lagrangian_tree map_fields(
	const model::lagrangian_tree &expr,
	const std::map<support::uuid, support::uuid> &field_id_map
) {
	return cxxmath::recursive_tree_transform( expr, map_fields_impl{ field_id_map } );
}
}
