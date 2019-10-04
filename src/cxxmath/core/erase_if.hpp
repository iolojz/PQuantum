//
// Created by jayz on 29.08.19.
//

#ifndef CXXMATH_CORE_ERASE_IF_HPP
#define CXXMATH_CORE_ERASE_IF_HPP

#ifndef __has_cpp_attribute
#define CXXMATH_USE_CXX20_ERASE_IF 0
#elif __has_cpp_attribute( __cpp_lib_erase_if )
#define CXXMATH_USE_CXX20_ERASE_IF 1
#else
#define CXXMATH_USE_CXX20_ERASE_IF 0
#endif

namespace cxxmath
{
#if CXXMATH_USE_CXX20_ERASE_IF
using std::erase_if;
#else

// This is equivalent to std::erase_if() in c++20
template<class ErasableRange, class Predicate>
void erase_if( ErasableRange &r, Predicate predicate )
{
	auto it = r.begin();
	auto end = r.end();
	
	while( it != end ) {
		if( predicate( *it )) {
			it = r.erase( it );
			end = r.end();
		} else
			++it;
	}
}

#endif
}

#endif //TAGD_CORE_ERASE_IF_HPP
