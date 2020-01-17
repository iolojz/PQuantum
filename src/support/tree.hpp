//
// Created by jayz on 10.01.20.
//

#ifndef PQUANTUM_SUPPORT_TREE_HPP
#define PQUANTUM_SUPPORT_TREE_HPP

#include <boost/hana.hpp>
#include <boost/tti/tti.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include "parsing_fwd.hpp"
#include "variant.hpp"
#include "is_member.hpp"

namespace PQuantum::support::tree {
namespace detail {
BOOST_TTI_HAS_STATIC_MEMBER_DATA( tree_node_arity )
}

struct runtime_arity_t {};
static constexpr auto runtime_arity = runtime_arity_t{};

// FIXME: We should really not require exact type matches... Especially not when they also require proper cv qualification!
template<class NodeData>
static constexpr bool is_terminal( boost::hana::basic_type<NodeData> node ) {
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

template<class NodeData>
static constexpr auto arity( boost::hana::basic_type<NodeData> node ) {
	return decltype(+node)::type::tree_node_arity;
}

template<class NodeData>
static constexpr bool has_exactly_one_child( boost::hana::basic_type<NodeData> node_data ) {
	if constexpr( is_terminal( node_data ) )
		return false;
	else {
		constexpr auto arity_ = arity( node_data );
		if constexpr( !std::is_same_v<decltype(arity_), const runtime_arity_t> )
			return (arity_ == 1);
		else
			return false;
	}
}

template<class NodeData, class TreeNode, class = void>
struct tree_node_incarnation;

template<class>
struct is_tree_node_incarnation : std::false_type {};
template<class Node, class TreeNode>
struct is_tree_node_incarnation<tree_node_incarnation<Node, TreeNode>> : std::true_type {};
template<class T> static constexpr auto is_tree_node_incarnation_v = is_tree_node_incarnation<T>::value;

namespace detail {
template<class NodeIncarnation> struct node_data_of_incarnation {
	using type = typename NodeIncarnation::node_data;
};
template<class NodeIncarnation>
using node_data_of_incarnation_t = typename node_data_of_incarnation<NodeIncarnation>::type;
}

template<class NodeData, class ...Incarnations>
static constexpr std::size_t index_of_node_incarnation( const boost::variant<Incarnations...> & ) {
	constexpr auto incarnations = boost::hana::tuple_t<
		typename boost::unwrap_recursive<Incarnations>::type...
	>;
	constexpr auto node_data_types = boost::hana::transform(
		incarnations,
		boost::hana::template_<detail::node_data_of_incarnation_t>
	);
	constexpr auto index = boost::hana::index_if(
		node_data_types,
		boost::hana::equal.to( boost::hana::type_c<NodeData> )
	);
	static_assert( boost::hana::is_just( index ), "NodeType does not exist in given IncarnationVariant" );
	return boost::hana::value( *index );
}

template<class NodeData, class ...Incarnations>
static constexpr bool holds_node_incarnation( const boost::variant<Incarnations...> &iv ) {
	return iv.which() == index_of_node_incarnation<NodeData>( iv );
}

template<class NodeData, class ...Incarnations>
static constexpr decltype(auto) get_node_incarnation( const boost::variant<Incarnations...> &iv ) {
	constexpr auto index = index_of_node_incarnation<NodeData>( iv );
	using incarnation = std::tuple_element_t<index, std::tuple<
		typename boost::unwrap_recursive<Incarnations>::type...
	>>;
	return boost::get<incarnation>( iv );
}

template<class NodeData, class ...Incarnations>
static constexpr decltype(auto) get_node_incarnation( boost::variant<Incarnations...> &iv ) {
	constexpr auto index = index_of_node_incarnation<NodeData>( iv );
	using incarnation = std::tuple_element_t<index, std::tuple<
		typename boost::unwrap_recursive<Incarnations>::type...
	>>;
	return boost::get<incarnation>( iv );
}

template<class ...Nodes>
class tree_node;

template<class>
struct is_tree_node : std::false_type {};
template<class ...Nodes>
struct is_tree_node<tree_node<Nodes...>> : std::true_type {};
template<class T> static constexpr auto is_tree_node_v = is_tree_node<T>::value;

namespace detail {
template<class T> class wrap_non_terminals;

template<class NodeData, class TreeNode>
class wrap_non_terminals<tree_node_incarnation<NodeData, TreeNode>> {
	using incarnation = tree_node_incarnation<NodeData, TreeNode>;
public:
	using type = std::conditional_t<
		is_terminal( boost::hana::type_c<NodeData> ),
		incarnation,
		boost::recursive_wrapper<incarnation>
	>;
};
template<class T> using wrap_non_terminals_t = typename wrap_non_terminals<T>::type;
}

template<class NodeData, class TreeNode>
static constexpr auto child_container_in_tree(
	boost::hana::basic_type<NodeData> node,
	boost::hana::basic_type<TreeNode>
) {
	constexpr auto valid_child_data_types = NodeData::valid_child_data_types( TreeNode::child_data_types );
	
	static_assert( boost::hana::length( valid_child_data_types ) != boost::hana::size_c<0>,
				   "No valid child data types." );
	
	constexpr auto incarnation_for_child_data_type = boost::hana::compose(
		boost::hana::template_<detail::wrap_non_terminals_t>,
		boost::hana::curry<2>(
			boost::hana::flip( boost::hana::template_<tree_node_incarnation> )
		)( boost::hana::type_c<TreeNode> )
	);
	
	using child_incarnation = typename decltype(+boost::hana::unpack(
		boost::hana::transform(
			valid_child_data_types,
			incarnation_for_child_data_type
		),
		boost::hana::template_<boost::variant>
	))::type;
	
	constexpr auto arity_ = arity( node );
	
	if constexpr( boost::hana::type_c<std::decay_t<decltype( arity_ )>> == boost::hana::type_c<runtime_arity_t> )
		return boost::hana::type_c<std::vector<child_incarnation>>;
	else if constexpr( arity_ != 1 )
		return boost::hana::type_c<std::array<child_incarnation, arity_>>;
	else
		return boost::hana::type_c<child_incarnation>;
}

// non-terminals, arity != 1
template<class NodeData, class TreeNode>
struct tree_node_incarnation<NodeData, TreeNode, std::enable_if_t<!is_terminal( boost::hana::type_c<NodeData> ) && !has_exactly_one_child( boost::hana::type_c<NodeData> )>> {
	NodeData data;
	typename decltype(+child_container_in_tree(
		boost::hana::type_c<NodeData>,
		boost::hana::type_c<TreeNode>
	))::type children;
	
	using node_data = NodeData;
	using child_type = typename decltype(children)::value_type;
	
	static constexpr bool is_terminal( void ) { return false; };
	
	tree_node_incarnation( void ) = default;
	tree_node_incarnation( tree_node_incarnation && ) = default;
	tree_node_incarnation( const tree_node_incarnation &other ) = default;
	
	template<class N, class C>
	tree_node_incarnation( N &&n, C &&c ) : data{ std::forward<N>( n ) }, children{ std::forward<C>( c ) } {}
	
	tree_node_incarnation &operator=( tree_node_incarnation && ) = default;
	tree_node_incarnation &operator=( const tree_node_incarnation & ) = default;
};


// non-terminals, arity == 1
template<class NodeData, class TreeNode>
struct tree_node_incarnation<NodeData, TreeNode, std::enable_if_t<has_exactly_one_child( boost::hana::type_c<NodeData> )>> {
	NodeData data;
	typename decltype(+child_container_in_tree(
		boost::hana::type_c<NodeData>,
		boost::hana::type_c<TreeNode>
	))::type child;
	
	using node_data = NodeData;
	using child_type = decltype(child);
	
	static constexpr bool is_terminal( void ) { return false; };
	
	tree_node_incarnation( void ) = default;
	tree_node_incarnation( tree_node_incarnation && ) = default;
	tree_node_incarnation( const tree_node_incarnation &other ) = default;
	
	template<class N, class C>
	tree_node_incarnation( N &&n, C &&c ) : data{ std::forward<N>( n ) }, child{ std::forward<C>( c ) } {}
	
	tree_node_incarnation &operator=( tree_node_incarnation && ) = default;
	tree_node_incarnation &operator=( const tree_node_incarnation & ) = default;
};

// terminals
template<class NodeData, class TreeNode>
struct tree_node_incarnation<NodeData, TreeNode, std::enable_if_t<is_terminal( boost::hana::type_c<NodeData> )>> {
	NodeData data;
	using node_data = NodeData;
	
	static constexpr bool is_terminal( void ) { return true; };
	
	tree_node_incarnation( void ) = default;
	tree_node_incarnation( tree_node_incarnation && ) = default;
	tree_node_incarnation( const tree_node_incarnation & ) = default;
	
	tree_node_incarnation( NodeData &&n ) : data{ std::move( n ) } {}
	tree_node_incarnation( const NodeData &n ) : data{ n } {}
	
	tree_node_incarnation &operator=( tree_node_incarnation && ) = default;
	tree_node_incarnation &operator=( const tree_node_incarnation & ) = default;
};

template<class ...NodeData>
struct tree_node {
	static constexpr auto child_data_types = boost::hana::tuple_t<NodeData...>;
	using node_variant = boost::variant<
		tree_node_incarnation<NodeData, tree_node>...
	>;
	node_variant node_incarnation;
	
	tree_node( void ) = default;
	tree_node( tree_node &&node ) = default;
	tree_node( const tree_node &node ) = default;
	
	template<class NodeIncarnation, std::enable_if_t<is_tree_node_incarnation_v<NodeIncarnation>, int> = 42>
	tree_node( NodeIncarnation &&ni ) : node_incarnation{ std::forward<NodeIncarnation>( ni ) } {}
	
	tree_node( node_variant &&node ) : node_incarnation{ std::move( node ) } {}
	tree_node( const node_variant &node ) : node_incarnation{ node } {}
	
	template<class ND, std::enable_if_t<is_member_v<std::decay_t<ND>, std::tuple<NodeData...>>, int> = 42>
	tree_node( ND &&nd ) : node_incarnation{
		tree_node_incarnation<std::decay_t<ND>, tree_node>{	std::forward<ND>( nd ) }
	} {}
	
	template<class ND, class Children>
	tree_node( ND &&nd, Children &&children ) : node_incarnation{
		tree_node_incarnation<std::decay_t<ND>, tree_node>{
			std::forward<ND>( nd ),
			std::forward<Children>( children )
		}
	} {}
	
	tree_node &operator=( tree_node &&node ) = default;
	tree_node &operator=( const tree_node &node ) = default;
	
	tree_node &operator=( node_variant &&node ) {
		node_incarnation = std::move( node );
		return *this;
	}
	
	tree_node &operator=( const node_variant &node ) {
		node_incarnation = node;
		return *this;
	}
};

template<class TreeNode>
std::enable_if_t<is_tree_node_v<std::decay_t<TreeNode>>, std::ostream &>
operator<<( std::ostream &os, const TreeNode &tn ) {
	return os << tn.node_incarnation;
}
}

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( NodeData )( TreeNode ),
	(PQuantum::support::tree::tree_node_incarnation) ( NodeData )( TreeNode )(
		std::enable_if_t<!is_terminal( boost::hana::type_c<NodeData> ) &&
		!has_exactly_one_child( boost::hana::type_c<NodeData> )>
	),
	data,
	children
)

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( NodeData )( TreeNode ),
	(PQuantum::support::tree::tree_node_incarnation) ( NodeData )( TreeNode )(
		std::enable_if_t<has_exactly_one_child( boost::hana::type_c<NodeData> )>
	),
	data,
	child
)

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( NodeData )( TreeNode ),
	(PQuantum::support::tree::tree_node_incarnation) ( NodeData )( TreeNode )(
		std::enable_if_t<is_terminal( boost::hana::type_c<NodeData> )> ),
	data
)

namespace PQuantum::support::parsing {
template<class Node>
struct rule_for_impl<Node, std::enable_if_t<tree::is_tree_node_v<Node>>> {
	static constexpr const char *name = "tree_node";
	
	static constexpr auto apply( void ) {
		using node_variant = typename Node::node_variant;
		return rule_for<node_variant>();
	}
};
}

#endif //PQUANTUM_SUPPORT_TREE_HPP
