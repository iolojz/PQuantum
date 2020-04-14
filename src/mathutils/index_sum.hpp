//
// Created by jayz on 14.04.20.
//

#ifndef PQUANTUM_MATHUTILS_INDEX_SUM_HPP
#define PQUANTUM_MATHUTILS_INDEX_SUM_HPP

namespace PQuantum::mathutils {
struct index_sum {
	std::set<support::uuid> indices;
	
	index_sum( void ) = default;
	index_sum( index_sum && ) = default;
	index_sum( const index_sum & ) = default;
	
	index_sum &operator=( index_sum & ) = default;
	index_sum &operator=( const index_sum & ) = default;
	
	index_sum( std::initializer_list<support::uuid> list ) : indices( list ) {}
	
	template<
	    class Arg,
	    std::enable_if_t<!std::is_same_v<std::decay_t<Arg>, index_sum>, int> = 42,
	    class = std::enable_if_t<std::is_constructible_v<decltype(indices), Arg &&>>
	> index_sum( Arg &&arg ) : indices( std::forward<Arg>( arg ) ) {}
	
	template<
	    class ...Args,
		class = std::enable_if_t<(sizeof...(Args) >= 2)>
	> index_sum( Args &&... args ) : indices( std::forward<Args>( args )... ) {}
	
	bool operator==( const index_sum &other ) const {
		return indices == other.indices;
	}
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const index_sum &is ) {
	os << "\\IndexSum{";
	
	if( std::empty( is.indices ) )
		return os << "}";
	
	auto index_it = std::begin( is.indices );
	os << *index_it;
	
	while( ++index_it != std::end( is.indices ) )
		os << ", " << *index_it;
	return os << "}";
}
}

#endif //PQUANTUM_MATHUTILS_INDEX_SUM_HPP
