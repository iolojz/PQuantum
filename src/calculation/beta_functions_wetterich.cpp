//
// Created by jayz on 02.04.20.
//

#include "calculation.hpp"

#include "lagrangian_derivative.hpp"

namespace PQuantum::calculation {
void calculate_beta_functions_with_wetterich_equation( const model::model_specification &model ) {
	auto lagrangian = model.model_lagrangian();
	auto second_derivative = take_nth_derivative( 2, take_nth_derivative( 1, lagrangian ) );
	
	std::cout << second_derivative << std::endl;
}
}