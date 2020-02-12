//
// Created by jayz on 11.02.20.
//

#ifndef PQUANTUM_PARSING_SCANNER_STATE_HPP
#define PQUANTUM_PARSING_SCANNER_STATE_HPP

#include <string>

namespace PQuantum::parsing {
struct scanner_state {
	std::string::const_iterator current;
	const std::string::const_iterator end;
};

struct input_location {
	std::decay_t<decltype(std::declval<scanner_state>().current)> begin, end;
};
}

#endif //PQUANTUM_PARSING_SCANNER_STATE_HPP
