//
// Created by jayz on 02.04.20.
//

#ifndef PQUANTUM_CALCULATION_CONTROLLER_HPP
#define PQUANTUM_CALCULATION_CONTROLLER_HPP

#include <vector>

#include "../model/model_specification.hpp"

namespace PQuantum {
enum class calculation_command {
	beta_functions
};
enum class calculation_method {
	wetterich
};

class calculation_controller {
	std::vector<calculation_command> commands;
	calculation_method method;
	
	logging::severity_logger logger;
	
	void calculate_beta_functions( const model::model_specification &model );
public:
	calculation_controller( const std::vector<calculation_command> &cs, calculation_method cm );
	
	void calculate( const model::model_specification &model );
};
}

#endif //PQUANTUM_CALCULATION_CONTROLLER_HPP
