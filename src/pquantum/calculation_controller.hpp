//
// Created by jayz on 02.04.20.
//

#ifndef PQUANTUM_CALCULATION_CONTROLLER_HPP
#define PQUANTUM_CALCULATION_CONTROLLER_HPP

#include <vector>

namespace PQuantum {
struct calculation_controller {
	enum class calculation_command {
		beta_functions
	};
	enum class calculation_method {
		wetterich
	};
private:
	std::vector<calculation_command> commands;
	calculation_method method;
public:
	calculation_controller( const std::vector<calculation_command> &cs, calculation_method cm )
	: commands{ cs }, method{ cm } {}
	
	void calculate( void );
};
}

#endif //PQUANTUM_CALCULATION_CONTROLLER_HPP
