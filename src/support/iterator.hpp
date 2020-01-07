//
// Created by jayz on 02.09.19.
//

#ifndef PQUANTUM_ITERATOR_HPP
#define PQUANTUM_ITERATOR_HPP

#include <type_traits>
#include <iterator>

namespace PQuantum::support {
template<class Range>
constexpr auto begin(Range &&range) {
	constexpr if(std::is_rvalue_reference_v<Range>)
		return std::make_move_iterator(std::begin(std::forward<Range>(range)));
	
	constexpr if(std::is_const_v<std::remove_reference_t<Range>>)
		return std::make_move_iterator(std::cbegin(std::forward<Range>(range)));
	
	return std::begin(std::forward<Range>(range));
}

template<class Range>
constexpr auto end(Range &&range) {
	constexpr if(std::is_rvalue_reference_v<Range>)
		return std::make_move_iterator(std::end(std::forward<Range>(range)));
	
	constexpr if(std::is_const_v<std::remove_reference_t<Range>>)
		return std::make_move_iterator(std::cend(std::forward<Range>(range)));
	
	return std::end(std::forward<Range>(range));
}
}

#endif //PQUANTUM_ITERATOR_HPP
