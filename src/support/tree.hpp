//
// Created by jayz on 10.01.20.
//

#ifndef PQUANTUM_SUPPORT_TREE_HPP
#define PQUANTUM_SUPPORT_TREE_HPP

#include <boost/hana.hpp>
#include <boost/tti/tti.hpp>

#include <boost/fusion/include/adapt_adt.hpp>

#include "parsing_fwd.hpp"
#include "std_variant.hpp"
#include "is_member.hpp"

namespace PQuantum::support::tree {
namespace detail {
BOOST_TTI_HAS_STATIC_MEMBER_DATA( tree_node_arity )
}

struct runtime_arity_t {};
static constexpr auto runtime_arity = runtime_arity_t{};

// FIXME: We should really not require excat type matches... Especially not when they also require proper cv qualification!
template<class Node>
static constexpr bool type_is_terminal( boost::hana::basic_type<Node> node ) {
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
static constexpr auto child_container( boost::hana::basic_type<Node> node, boost::hana::basic_type<Child> ) {
	constexpr auto arity_ = arity( node );
	
	if constexpr( boost::hana::type_c<std::decay_t<decltype( arity_ )>> == boost::hana::type_c<runtime_arity_t> )
		return boost::hana::type_c<std::vector<Child>>;
	else
		return boost::hana::type_c<std::array<Child, arity_>>;
}

// non-terminals
template<class Node, class Child = void>
class tree_node_incarnation {
	static_assert( !type_is_terminal( boost::hana::type_c<Node> ), "A non-void Child must have a non-terminal Node" );
	std::optional<Node> data_;
	typename decltype(+child_container( boost::hana::type_c<Node>, boost::hana::type_c<Child> ))::type children_;
public:
	using node_type = Node;
	using child_type = Child;
	static constexpr bool is_terminal = false;
	
	tree_node_incarnation( void ) = default;
	
	tree_node_incarnation( tree_node_incarnation &&t ) = default;
	
	tree_node_incarnation( const tree_node_incarnation & ) = default;
	
	template<class N, class C>
	tree_node_incarnation( N &&n, C &&c ) : data_{ std::forward<N>( n ) }, children_{ std::forward<C>( c ) } {}
	
	tree_node_incarnation &operator=( tree_node_incarnation && ) = default;
	
	tree_node_incarnation &operator=( const tree_node_incarnation & ) = default;
	
	void set_data( Node &&n ) { data_ = std::move( n ); }
	
	void set_data( const Node &n ) { data_ = n; }
	
	auto &data( void ) { return *data_; }
	
	const auto &data( void ) const { return *data_; }
	
	template<class C>
	void set_children( C &&c ) { children = std::forward<C>( c ); }
	
	auto &children( void ) { return *data_; }
	
	const auto &children( void ) const { return *data_; }
};

// terminals
template<class Node>
class tree_node_incarnation<Node, void> {
	static_assert( is_terminal( boost::hana::type_c<Node> ), "A void Child must have a terminal Node" );
	std::optional<Node> data_;
public:
	using node_type = Node;
	static constexpr bool is_terminal = true;
	
	tree_node_incarnation( void ) = default;
	
	tree_node_incarnation( tree_node_incarnation && ) = default;
	
	tree_node_incarnation( const tree_node_incarnation & ) = default;
	
	tree_node_incarnation( Node &&n ) : data_{ std::move( n ) } {}
	
	tree_node_incarnation( const Node &n ) : data_{ n } {}
	
	tree_node_incarnation &operator=( tree_node_incarnation && ) = default;
	
	tree_node_incarnation &operator=( const tree_node_incarnation & ) = default;
	
	void set_data( Node &&n ) { data_ = std::move( n ); }
	
	void set_data( const Node &n ) { data_ = n; }
	
	auto &data( void ) { return *data_; }
	
	const auto &data( void ) const { return *data_; }
};

template<class>
struct is_tree_node_incarnation : std::false_type {};
template<class Node, class Child>
struct is_tree_node_incarnation<tree_node_incarnation<Node, Child>> : std::true_type {};
template<class T> static constexpr auto is_tree_node_incarnation_v = is_tree_node_incarnation<T>::value;

template<class ...Nodes>
class tree_node;

template<class>
struct is_tree_node : std::false_type {};
template<class ...Nodes>
struct is_tree_node<tree_node<Nodes...>> : std::true_type {};
template<class T> static constexpr auto is_tree_node_v = is_tree_node<T>::value;

template<class ...NodeData>
class tree_node {
public:
	using node_variant = std::variant<
		tree_node_incarnation<NodeData, tree_node>...
	>;
private:
	std::optional<node_variant> data_;
public:
	tree_node( void ) = default;
	
	tree_node( tree_node &&node ) = default;
	
	tree_node( const tree_node &node ) = default;
	
	template<class NodeIncarnation, std::enable_if_t<is_tree_node_incarnation_v<NodeIncarnation>, int> = 42>
	tree_node( NodeIncarnation &&ni ) : data_{ std::forward<NodeIncarnation>( ni ) } {}
	
	tree_node( node_variant &&node ) : data_{ std::move( node ) } {}
	
	tree_node( const node_variant &node ) : data_{ node } {}
	
	template<class ND, std::enable_if_t<is_member_v<std::decay_t<ND>, std::tuple<NodeData...>>, int> = 42>
	tree_node( ND &&nd ) : data_{
		std::in_place,
		std::in_place_type<tree_node_incarnation<std::decay_t<ND>>>,
		std::forward<ND>( nd )
	} {}
	
	template<class ND, class Children>
	tree_node( ND &&nd, Children &&children ) : data_{
		std::in_place,
		std::in_place_type<tree_node_incarnation<std::decay_t<ND>, tree_node>>,
		std::forward<ND>( nd ), std::forward<Children>( children )
	} {}
	
	tree_node &operator=( tree_node &&node ) = default;
	
	tree_node &operator=( const tree_node &node ) = default;
	
	tree_node &operator=( node_variant &&node ) {
		data_ = std::move( node );
		return *this;
	}
	
	tree_node &operator=( const node_variant &node ) {
		data_ = node;
		return *this;
	}
	
	node_variant &data( void ) { return *data_; }
	
	const node_variant &data( void ) const { return *data_; }
};
}

BOOST_FUSION_ADAPT_TPL_ADT(
	( Node ),
	(PQuantum::support::tree::tree_node_incarnation) ( Node ),
	( obj.data(), obj.set_data( val ))
)

BOOST_FUSION_ADAPT_TPL_ADT(
	( Node )( Child ),
	(PQuantum::support::tree::tree_node_incarnation) ( Node )( Child ),
	( obj.data(), obj.set_data( val ))
		( obj.children(), obj.set_children( val ))
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
