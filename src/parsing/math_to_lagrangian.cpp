//
// Created by jayz on 13.02.20.
//

#include "math_to_lagrangian.hpp"

#include <boost/variant/static_visitor.hpp>

#include <boost/tti/has_static_member_data.hpp>

using namespace PQuantum;

namespace {
BOOST_TTI_HAS_STATIC_MEMBER_DATA(value)

class converter : public boost::static_visitor<model::lagrangian_node> {
	template<class T> using old_node_incarnation = support::tree::node_incarnation<T, parsing::math_tree_tag>;
	template<class T> using new_node_incarnation = support::tree::node_incarnation<T, model::lagrangian_tree_tag>;
	
	parsing::qft_parsing_context &context;
	
	template<class NewContainer, class OldContainer, class ...Unpacked>
	auto self_apply_unpack( OldContainer &&old_container, Unpacked &&...unpacked ) const {
		if constexpr( sizeof...(Unpacked) == std::tuple_size<std::decay_t<OldContainer>>::value )
			return NewContainer{ boost::apply_visitor( *this, std::move( unpacked ) )... };
		else
			return self_apply_unpack<NewContainer>(
				std::forward<OldContainer>( old_container ),
				std::forward<Unpacked>( unpacked )...,
				std::get<sizeof...(Unpacked)>( std::forward<OldContainer>( old_container ) )
			);
	}
	
	template<class NewContainer, class OldContainer>
	auto convert_constant_size_container( OldContainer &&old_container ) const {
		return self_apply_unpack<NewContainer>( std::forward<OldContainer>( old_container ) );
	}
	
	template<class NewContainer, class OldContainer>
	auto convert_runtime_size_container( OldContainer &&old_container ) const {
		NewContainer new_container;
		if constexpr( std::is_rvalue_reference_v<OldContainer &&> )
			std::transform(
				std::make_move_iterator( std::begin( old_container ) ),
				std::make_move_iterator( std::end( old_container ) ),
				std::back_inserter( new_container ),
				[this] ( auto &&v ) { return boost::apply_visitor( *this, std::forward<decltype(v)>( v ) ); }
			);
		else
			std::transform(
				std::begin( old_container ),
				std::end( old_container ),
				std::back_inserter( new_container ),
				[this] ( auto &&v ) { return boost::apply_visitor( *this, std::forward<decltype(v)>( v ) ); }
			);
		return new_container;
	}
	
	template<class NewContainer, class OldContainer>
	auto convert_container( OldContainer &&old_container ) const {
		if constexpr( has_static_member_data_value<std::tuple_size<OldContainer>, const std::size_t>::value ) {
			static_assert( has_static_member_data_value<std::tuple_size<NewContainer>, const std::size_t>::value, "Internal error" );
			return convert_constant_size_container<NewContainer>( std::forward<OldContainer>( old_container ) );
		} else {
			static_assert( !has_static_member_data_value<std::tuple_size<NewContainer>, const std::size_t>::value, "Internal error" );
			return convert_runtime_size_container<NewContainer>( std::forward<OldContainer>( old_container ) );
		}
	}
public:
	converter( parsing::qft_parsing_context &c ) : context{ c } {}
	
	constexpr auto operator()( boost::blank ) const {
		return boost::blank{};
	}
	
	template<class OldNodeData> auto operator()( old_node_incarnation<OldNodeData> &&node ) const {
		using new_node_type = new_node_incarnation<OldNodeData>;
		using new_child_container = typename new_node_type::child_container;
		
		return new_node_type{ std::move(node.data), convert_container<new_child_container>(std::move(node.children)) };
	}
};

template<> auto converter::operator()<mathutils::atom_with_optional_indices<mathutils::string_atom>>(
	old_node_incarnation<mathutils::atom_with_optional_indices<mathutils::string_atom>> &&indexed_atom_node
) const {
	using new_node_type = boost::variant<
	    new_node_incarnation<model::indexed_field>,
		new_node_incarnation<model::parameter>
	>;
	mathutils::index_spec<support::uuid> indices;
	
	for( auto &&index : indexed_atom_node.data.indices.lower ) {
		// TODO: Assert the index to be an identifier and not a number
		indices.lower.push_back( context.index_id_lookup_and_generate( index.name ) );
	}
	for( auto &&index : indexed_atom_node.data.indices.upper ) {
		// TODO: Assert the index to be an identifier and not a number
		indices.upper.push_back( context.index_id_lookup_and_generate( index.name ) );
	}
	
	std::optional<support::uuid> id;
	if( (id = context.field_id_lookup( indexed_atom_node.data.atom.name )) )
		return new_node_type{ new_node_incarnation<model::indexed_field>{ model::indexed_field{ *id, std::move( indices ) } } };
	else if( (id = context.field_id_lookup( indexed_atom_node.data.atom.name )) ) {
		// TODO: Assert the indices to be empty
		return new_node_type{ new_node_incarnation<model::parameter>{ model::parameter{ *id } } };
	}
	
	// TODO: Report some meaningful error
	throw( "foo" );
}
}

namespace PQuantum::parsing {
model::lagrangian_node math_to_lagrangian( math_tree_node &&math_node, qft_parsing_context &&qft_context ) {
	return boost::apply_visitor( converter{ qft_context }, std::move( math_node ) );
}
}
