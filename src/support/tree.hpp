//
// Created by jayz on 10.01.20.
//

#ifndef PQUANTUM_SUPPORT_TREE_HPP
#define PQUANTUM_SUPPORT_TREE_HPP

#include <boost/hana.hpp>
#include <boost/tti/tti.hpp>

#include "parsing.hpp"

#include <cxxmath/cxxmath.hpp>

namespace PQuantum::support::tree {
namespace detail {
BOOST_TTI_HAS_STATIC_MEMBER_DATA( tree_node_arity )
}

struct runtime_arity_t {};
static constexpr auto runtime_arity = runtime_arity_t{};

// FIXME: We should really not require excat type matches... Especially not when they also require proper cv qualification!
template<class Node>
static constexpr bool is_terminal( boost::hana::basic_type<Node> node ) {
	if constexpr( detail::has_static_member_data_tree_node_arity<typename decltype(+node)::type, const std::size_t>::value ) {
		if constexpr( decltype(+node)::type::tree_node_arity == 0 )
			return true;
		else
			return false;
	} else if constexpr( detail::has_static_member_data_tree_node_arity<typename decltype(+node)::type, const runtime_arity_t>::value )
		return false;
	else
		return true;
}

template<class Node>
static constexpr auto arity( boost::hana::basic_type<Node> node ) {
	return decltype(+node)::type::tree_node_arity;
}

template<class Node, class Child>
static constexpr auto child_container( boost::hana::basic_type<Node> node, boost::hana::basic_type<Child> child ) {
	using child_t = typename decltype(+child)::type;
	constexpr auto arity_ = arity( node );
	
	if constexpr( boost::hana::type_c<std::decay_t<decltype( arity_ )>> == boost::hana::type_c<runtime_arity_t> )
		return boost::hana::type_c<std::vector<child_t>>;
	else
		return boost::hana::type_c<std::array<child_t, arity_>>;
}

namespace detail {
template<bool is_terminal, class Child, class Node>
struct tree_node {
	Node data;
};

template<class Child, class Node>
struct tree_node<false, Child, Node> {
	Node data;
	typename decltype(+child_container( boost::hana::type_c<Node>, boost::hana::type_c<Child> ))::type children;
};
}

template<class ...Nodes>
struct tree_node {
	std::variant<detail::tree_node<is_terminal( boost::hana::type_c<Nodes> ), tree_node, Nodes>...> data;
	
	tree_node( void ) = default;
	
	template<class Node, class Children>
	tree_node( Node &&node, Children &&children ) : data{
		detail::tree_node<false, tree_node<Nodes...>, Node>{
			std::forward<Node>( node ),
			std::forward<Children>( children )
		}
	} {
		static_assert( !is_terminal( boost::hana::type_c<Node> ), "Attempt to attach children to terminal" );
	}
	
	template<class Node>
	tree_node( Node &&node ) : data{
		detail::tree_node<true, tree_node<Nodes...>, Node>{
			std::forward<Node>( node )
		}
	} {
		static_assert( is_terminal( boost::hana::type_c<Node> ), "Attempt to create childless non-terminal" );
	}
};
}

#endif //PQUANTUM_SUPPORT_TREE_HPP
