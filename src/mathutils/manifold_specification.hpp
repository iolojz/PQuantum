//
// Created by jayz on 31.07.19.
//

#ifndef PQUANTUM_MANIFOLD_SPECIFICATION_HPP
#define PQUANTUM_MANIFOLD_SPECIFICATION_HPP

#include <boost/property_tree/ptree_fwd.hpp>
#include <variant>

#include "error/control.hpp"
#include "io/logging.hpp"

namespace PQuantum
{
namespace mathutils
{
struct spacetime_dimension
{
	bool operator==( const spacetime_dimension & ) const
	{ return true; }
	
	bool operator<( const spacetime_dimension & ) const
	{ return false; }
};

namespace manifold_types
{
struct vector_space
{
	enum class algebraic_field
	{
		real, complex, complex_grassmann
	} field;
	std::variant<int, spacetime_dimension> dimension;
	
	enum class vector_space_metric
	{
		unspecified, euclidean, //< (+, ..., +)
		lorentzian, //< (+, -, ..., -)
		complex_nondegenerate
	} metric;
	
	vector_space( algebraic_field f, std::variant<int, spacetime_dimension> d, vector_space_metric m ) : field{ f },
																										 dimension{ d },
																										 metric{ m }
	{
		BOOST_LOG_NAMED_SCOPE( "vector_space::vector_space()" );
		io::severity_logger logger;
		
		if( std::holds_alternative<int>( dimension ) && std::get<int>( dimension ) < 0 ) {
			BOOST_LOG_SEV( logger, io::severity_level::error ) << "Negative dimension specified: "
															   << std::get<int>( dimension );
			control::exit_upon_error();
		}
		
		switch( metric ) {
			case vector_space_metric::euclidean:
			case vector_space_metric::lorentzian:
				if( field != algebraic_field::real ) {
					BOOST_LOG_SEV( logger, io::severity_level::error )
						<< "Only real vector spaces can have a euclidean metric.";
					control::exit_upon_error();
				}
				break;
			case vector_space_metric::complex_nondegenerate:
				if( field != algebraic_field::complex ) {
					BOOST_LOG_SEV( logger, io::severity_level::error )
						<< "Only complex vector spaces can have a complex nondegenerate metric.";
					control::exit_upon_error();
				}
				break;
			case vector_space_metric::unspecified:
				break;
		}
	}
};

std::ostream &
operator<<( std::ostream &os, const PQuantum::mathutils::manifold_types::vector_space::algebraic_field &field );

std::ostream &
operator<<( std::ostream &os, const PQuantum::mathutils::manifold_types::vector_space::vector_space_metric &metric );
}

using manifold_specification = std::variant<manifold_types::vector_space>;
}
}


#endif //PQUANTUM_MANIFOLD_SPECIFICATION_HPP
