//
// Created by jayz on 23.08.19.
//

#ifndef PQUANTUM_PARSING_PARSER_RULES_RULE_FOR_FWD_HPP
#define PQUANTUM_PARSING_PARSER_RULES_RULE_FOR_FWD_HPP

namespace PQuantum::parsing::parser_rules {
template<class T, class = void>
struct rule_for_impl;
template<class T> static constexpr auto rule_for = rule_for_impl<T>{};
}

#endif //PQUANTUM_PARSING_PARSER_RULES_RULE_FOR_FWD_HPP
