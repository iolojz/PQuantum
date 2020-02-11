//
// Created by jayz on 10.01.20.
//

#ifndef PQUANTUM_SUPPORT_TREE_HPP
#define PQUANTUM_SUPPORT_TREE_HPP

#include <boost/array.hpp>

#include <boost/hana.hpp>
#include <boost/hana/ext/boost/mpl.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/boost_array.hpp>

#include <boost/variant.hpp>

namespace PQuantum::support::tree {
struct runtime_arity_t {};
static constexpr auto runtime_arity = runtime_arity_t{};

template<class T> constexpr auto arity_for_node_data( boost::hana::basic_type<T> );

template<class T>
static constexpr auto is_terminal( boost::hana::basic_type<T> t ) {
	constexpr auto arity = arity_for_node_data( t );
	if constexpr( boost::hana::typeid_( arity ) == boost::hana::type_c<runtime_arity_t> )
		return boost::hana::false_c;
	else
		return boost::hana::bool_c<arity == 0>;
}

template<class T, class TreeTag, class = void> class node_incarnation {
	static_assert( is_terminal( boost::hana::type_c<T> ), "Internal error" );
public:
	using tree_node = TreeTag;
	using node_data = T;
	T data;
};

template<class T, class TreeTag>
class node_incarnation<T, TreeTag, std::enable_if_t<!is_terminal( boost::hana::type_c<T> )>> {
	static constexpr auto child_container_( void ) {
		constexpr auto arity = arity_for_node_data( boost::hana::type_c<T> );
		using node = typename TreeTag::node;
		
		if constexpr( boost::hana::typeid_( arity ) == boost::hana::type_c<runtime_arity_t> )
			return boost::hana::type_c<std::vector<node>>;
		else {
			static_assert( arity != 0, "Internal error" );
			return boost::hana::type_c<boost::array<node, arity>>;
		}
	}
public:
	template<class Data, class ...Children> node_incarnation( Data &&d, Children &&...ch )
	: data{ std::forward<Data>( d ) }, children{ std::forward<Children>( ch )... } {}
	
	using tree_tag = TreeTag;
	using node_data = T;
	using child_container = typename decltype(+child_container_())::type;
	
	node_data data;
	child_container children;
};

template<class ...NodeTraits>
class tree_tag {
	static_assert( sizeof...( NodeTraits ) != 0, "No node traits were provided." );
	
	static constexpr auto node_data_types = boost::hana::fold_left(
		boost::hana::make_tuple( NodeTraits::node_data_types... ),
		boost::hana::make_tuple(),
		boost::hana::concat
	);
	
	static constexpr auto node_data_types2 = boost::hana::transform(
		node_data_types,
		boost::hana::typeid_
	);
	static_assert( node_data_types == node_data_types2, "Some node data types are not boost::hana types." );
	
	static constexpr auto node_incarnation_in_tree = []( auto &&t ) {
		return boost::hana::type_c<node_incarnation<typename decltype(+t)::type, tree_tag>>;
	};
	static constexpr auto wrap_if_nonterminal = [] ( auto &&t ) {
		if constexpr( is_terminal( boost::hana::type_c<typename decltype(+t)::type> ) )
			return node_incarnation_in_tree( boost::hana::type_c<typename decltype(+t)::type> );
		else
			return boost::hana::type_c<boost::recursive_wrapper<typename decltype(+node_incarnation_in_tree(t))::type>>;
	};
	
	static constexpr auto wrapped_node_incarnations = boost::hana::transform(
		node_data_types,
		wrap_if_nonterminal
	);
public:
	using node = typename decltype(+boost::hana::unpack(
		/*boost::hana::prepend(
			wrapped_node_incarnations,
			boost::hana::type_c<boost::blank> // Make sure the variant is default-constructible
		),*/
		wrapped_node_incarnations,
		boost::hana::template_<boost::variant>
	))::type;
};

template<class ...NodeTraits> using tree_node = typename tree_tag<NodeTraits...>::node;

template<class NewTreeTag, class IncarnationVariant> class rebind_tree_tag {
	// TODO: add some assertions
	
	static constexpr auto wrapped_incarnations = boost::hana::to_tuple( typename IncarnationVariant::types{} );
	static constexpr auto incarnations = boost::hana::transform(
		wrapped_incarnations,
		boost::hana::metafunction<boost::unwrap_recursive>
	);
	static constexpr auto wrapped_rebound_incarnations = boost::hana::transform(
		incarnations,
		[] ( auto &&t ) {
			if constexpr( is_terminal( boost::hana::type_c<typename decltype(+t)::type::node_data> ) )
				return boost::hana::type_c<node_incarnation<typename decltype(+t)::type::node_data, NewTreeTag>>;
			else
				return boost::hana::type_c<boost::recursive_wrapper<node_incarnation<typename decltype(+t)::type::node_data, NewTreeTag>>>;
		}
	);
	using new_wrapped_incarnations = decltype(boost::hana::to<boost::hana::ext::boost::mpl::vector_tag>(
		wrapped_rebound_incarnations
	));
public:
	using type = typename boost::make_variant_over<new_wrapped_incarnations>::type;
};

template<class T, class IncarnationVariant>
static constexpr int index_of_node_data( boost::hana::basic_type<IncarnationVariant> ) {
	constexpr auto index = boost::hana::index_if(
		typename std::decay_t<IncarnationVariant>::types{},
		[] ( auto &&t ) {
			if constexpr( t == boost::hana::type_c<boost::blank> )
				return boost::hana::bool_c<std::is_same_v<T, boost::blank>>;
			else {
				using node_data = typename boost::unwrap_recursive<typename decltype(+t)::type::node_data>::type;
				return boost::hana::bool_c<std::is_same_v<T, node_data>>;
			}
		}
	);
	static_assert( boost::hana::is_just( index ),
		"The given node data type does not appear in the given incarnation variant." );
	return *index;
}

template<class T, class IncarnationVariant>
static constexpr bool holds_node_incarnation( const IncarnationVariant &iv ) {
	constexpr auto node_type_index = index_of_node_data<T>( boost::hana::type_c<IncarnationVariant> );
	return (node_type_index == iv.which());
}

template<class T, class IncarnationVariant>
static constexpr auto get_node_incarnation( IncarnationVariant &&iv ) {
	using incarnations = typename std::decay_t<IncarnationVariant>::types;
	constexpr auto node_type_index = index_of_node_data<T>( boost::hana::type_c<IncarnationVariant> );
	using incarnation = typename boost::mpl::at_c<incarnations, node_type_index>::type;
	return boost::get<incarnation>( std::forward<IncarnationVariant>( iv ) );
}
}

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( T )( TreeTag ),
	(PQuantum::support::tree::node_incarnation) ( T )( TreeTag )(
		std::enable_if_t<PQuantum::support::tree::is_terminal( boost::hana::type_c<T> )>
	),
	data
)

BOOST_FUSION_ADAPT_TPL_STRUCT(
	( T )( TreeTag ),
	(PQuantum::support::tree::node_incarnation) ( T )( TreeTag )(
		std::enable_if_t<!PQuantum::support::tree::is_terminal( boost::hana::type_c<T> )>
	),
	data,
	children
)

#define PQUANTUM_TREE_DEFINE_NODE_ARITY(type,arity) \
namespace PQuantum::support::tree { \
template<> constexpr auto arity_for_node_data<type>( boost::hana::basic_type<type> ) { \
    return arity; \
} \
}

#endif //PQUANTUM_SUPPORT_TREE_HPP
