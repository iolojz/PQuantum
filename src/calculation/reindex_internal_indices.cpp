//
// Created by jayz on 14.04.20.
//

#include "lagrangian_manipulations.hpp"

#include <stack>

namespace {
using namespace PQuantum;

std::vector<std::variant<int, support::uuid>> reindex(
	const std::vector<std::variant<int, support::uuid>> &indices,
	std::map<support::uuid, support::uuid> index_id_map
) {
	std::vector<std::variant<int, support::uuid>> reindexed = indices;
	for( auto &index : reindexed ) {
		if( std::holds_alternative<support::uuid>( index ) ) {
			auto index_id = std::get<support::uuid>( index );
			auto index_id_it = index_id_map.find( index_id );
			
			if( index_id_it != std::end( index_id_map ) )
				index = index_id_it->second;
		}
	}
	return reindexed;
}

class reindex_impl {
	const std::function<support::uuid(void)> &uuid_gen;
	mutable std::map<support::uuid, support::uuid> index_id_map;
	mutable std::stack<std::set<support::uuid>> pushed_indices;
public:
	reindex_impl( const std::function<support::uuid(void)> &u )
	: uuid_gen( u ) {}
	
	model::lagrangian_tree operator()( const model::indexed_field &ifield ) const {
		return model::indexed_field{
			ifield.id,
			{
				reindex( ifield.indices.lower, index_id_map ),
				reindex( ifield.indices.upper, index_id_map )
			}
		};
	}
	
	model::lagrangian_tree operator()( const model::indexed_parameter &ip ) const {
		return model::indexed_parameter{
			ip.id,
			{
				reindex( ip.indices.lower, index_id_map ),
				reindex( ip.indices.upper, index_id_map )
			}
		};
	}
	
	template<
	    class Atom,
	    class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, model::indexed_field>>,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, model::indexed_parameter>>,
		class = std::enable_if_t<!std::is_same_v<std::decay_t<Atom>, cxxmath::context_stack_push>>
	> model::lagrangian_tree operator()( Atom &&atom ) const {
		return std::forward<Atom>( atom );
	}
	
	void operator()( cxxmath::context_stack_push, const mathutils::index_sum &is ) const {
		for( const auto &index : is.indices )
			index_id_map.emplace( index, uuid_gen() );
		pushed_indices.push( is.indices );
	}
	
	void operator()( cxxmath::context_stack_pop ) const {
		for( const auto &index : pushed_indices.top() )
			index_id_map.erase( index );
		pushed_indices.pop();
	}
	
	template<
	    class NonTerminalNodeData,
	    class Args,
	    class TransformedArgs
	> model::lagrangian_tree operator()( NonTerminalNodeData &&nt, Args &&, TransformedArgs &&ta ) const {
		return {
			std::forward<NonTerminalNodeData>( nt ),
			std::forward<TransformedArgs>( ta )
		};
	}
};
}

namespace PQuantum::calculation {
model::lagrangian_tree reindex_internal_indices(
	const model::lagrangian_tree &expr,
	const std::function<support::uuid(void)> &uuid_gen
) {
	return cxxmath::recursive_tree_transform( expr, reindex_impl{ uuid_gen } );
}
}
