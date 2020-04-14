//
// Created by jayz on 02.04.20.
//

#include "calculation_controller.hpp"

#include <boost/log/attributes.hpp>

#include "../calculation/calculation.hpp"

namespace {
bool sort_calculation_commands( PQuantum::calculation_command, PQuantum::calculation_command ) {
	return false;
}
}

namespace PQuantum {
calculation_controller::calculation_controller( const std::vector<calculation_command> &cs, calculation_method cm )
: commands{ cs }, method{ cm } {
	std::sort( commands.begin(), commands.end(), sort_calculation_commands );
	commands.erase( std::unique( commands.begin(), commands.end() ), commands.end() );
	
	logger.add_attribute( "object", boost::log::attributes::constant<std::string>( "calculation_controller" ) );
}

void calculation_controller::calculate( const model::model_specification &model ) {
	BOOST_LOG_NAMED_SCOPE( "calculate()" );
	BOOST_LOG_SEV( logger, logging::severity_level::user_info ) << "Begin calculation!";
	
	for( calculation_command c : commands ) {
		switch( c ) {
		case calculation_command::beta_functions:
			calculate_beta_functions( model );
			break;
		default:
			throw std::runtime_error( "This cannot happen!" );
		}
	}
}

void calculation_controller::calculate_beta_functions( const model::model_specification &model ) {
	BOOST_LOG_NAMED_SCOPE( "calculate_beta_functions()" );
	BOOST_LOG_SEV( logger, logging::severity_level::user_info ) << "Calculating beta functions...";
	
	switch( method ) {
	case calculation_method::wetterich:
		calculation::wetterich::calculate_beta_functions( model );
		break;
	default:
		throw std::runtime_error( "This cannot happen!" );
	}
}
}
