//
// Created by jayz on 11.02.20.
//

#ifndef PQUANTUM_PARSING_SCANNER_STATE_HPP
#define PQUANTUM_PARSING_SCANNER_STATE_HPP

namespace PQuantum::parsing {
struct scanner_state {
	std::string::const_iterator current, end;
};
}

#endif //PQUANTUM_PARSING_SCANNER_STATE_HPP
