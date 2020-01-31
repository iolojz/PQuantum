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
#include "std_unique_ptr.hpp"
#include "is_member.hpp"

namespace PQuantum::support::tree {
struct runtime_arity_t {};
static constexpr auto runtime_arity = runtime_arity_t{};

static constexpr void arity_for_node_data( void ) {}

template<class T>
static constexpr bool is_terminal(boost::hana::basic_type<T> t) {
	constexpr auto arity = arity_for_node_data(t);
	if constexpr(decltype_(arity) == boost::hana::type<runtime_arity_t>)
		return false;
	else
		return (arity == 0);
}

template<class T, class TreeNode, class = void> class node_incarnation {
	static constexpr auto child_container( void ) {
		constexpr auto arity = arity_for_node_data(t);
		using node = typename TreeNode::type;
		
		if constexpr( boost::hana::typeid_(arity) == boost::hana::type<runtime_arity_t> )
			return std::vector<node>;
		else {
			static_assert(arity != 0, "Internal error");
			return std::array<node, arity>;
		}
	}
public:
	using node_data = T;
	using child_container = typename decltype(+child_container())::type;
	
	T data;
	child_container children;
};

template<class T> class node_incarnation<T, TreeNode, std::enable_if_t<is_terminal( boost::hana::type_c<T>() )>>> {
public:
	T data;
};

namespace detail {
template<class ...NodeTraits>
class tree_node {
	static_assert(sizeof...(NodeTraits) != 0, "No node traits were provided.");
	
	static constexpr auto node_data_types = boost::hana::fold_left(
		boost::hana::make_tuple(NodeTraits::node_data_types...),
		boost::hana::make_tuple(),
		boost::hana::concat
	);
	
	static constexpr auto node_data_types2 = boost::hana::transform(
		node_data_types,
		boost::hana::typeid_
	);
	static_assert( node_data_types == node_data_types2, "Some node data types are not boost::hana types." );
	
	static constexpr auto node_incarnation_in_tree = []( auto &&t ) {
		return boost::hana::type_c<typename decltype(+t)::type, tree_node>;
	}
	
	static constexpr auto wrapped_node_incarnations = boost::hana::transform(
		node_data_types,
		[]( auto &&t ) {
			if constexpr( is_terminal(t) )
				return node_incarnation_in_tree<NodeTraits...>( t );
			else {
				using type = typename decltype(+node_incarnation_in_tree(t))::type;
				return boost::hana::type_c<boost::recursive_wrapper<type>>;
			}
		}
	);
public:
	using type = typename decltype(+boost::hana::unpack(
		wrapped_node_incarnations,
		boost::hana::template_<boost::variant>
	 ))::type;
};
}

template<class ...NodeTraits> using tree_node = typename detail::tree_node<NodeTraits...>::type;

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( T )( TreeNode ),
	(PQuantum::support::tree::node_incarnation) ( T )( TreeNode )(
		std::enable_if_t<!PQuantum::support::tree::is_terminal( boost::hana::type_c<T> )> )
	),
	data,
	children
)

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( T )( TreeNode ),
	(PQuantum::support::tree::node_incarnation) ( T )( TreeNode )(
		std::enable_if_t<PQuantum::support::tree::is_terminal( boost::hana::type_c<T> )> )
	),
	data
)

#endif //PQUANTUM_SUPPORT_TREE_HPP
