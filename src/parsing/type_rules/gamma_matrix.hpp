//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_TYPE_RULES_GAMMA_MATRIX_HPP
#define PQUANTUM_PARSING_TYPE_RULES_GAMMA_MATRIX_HPP

namespace PQuantum::parsing::type_rules {
struct rule_for_impl<mathutils::linear_operators::gamma_matrix> {
	static constexpr std::string_view = "gamma_matrix";
	
	template<class Context>
	static auto apply( Context context ) {
		return "\\gamma" >> rule_for<mathutils::spacetime_index>( context );
	}
}
}

#endif //PQUANTUM_PARSING_TYPE_RULES_GAMMA_MATRIX_HPP
