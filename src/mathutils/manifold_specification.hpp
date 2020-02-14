//
// Created by jayz on 31.07.19.
//

#ifndef PQUANTUM_MANIFOLD_SPECIFICATION_HPP
#define PQUANTUM_MANIFOLD_SPECIFICATION_HPP

#include <boost/property_tree/ptree_fwd.hpp>
#include <variant>

#include "error/error.hpp"
#include "logging/logging.hpp"

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

struct clifford_algebra_target_dimension {};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const spacetime_dimension & ) {
	return os << "D";
}

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const clifford_algebra_target_dimension & ) {
	return os << "d_{\\gamma}";
}

namespace manifold_types
{
struct vector_space
{
	enum class algebraic_field
	{
		real, complex, complex_grassmann
	} field;
	std::variant<int, spacetime_dimension, clifford_algebra_target_dimension> dimension;
	
	enum class vector_space_metric
	{
		unspecified, euclidean, //< (+, ..., +)
		lorentzian, //< (+, -, ..., -)
		complex_nondegenerate
	} metric;
	
	vector_space( algebraic_field f, std::variant<int, spacetime_dimension, clifford_algebra_target_dimension> d, vector_space_metric m )
	: field{ f }, dimension{ d },  metric{ m } {
		BOOST_LOG_NAMED_SCOPE("vector_space::vector_space()");
		logging::severity_logger logger;
		
		if(std::holds_alternative<int>(dimension) && std::get<int>(dimension) < 0) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "Negative dimension specified: "
																  << std::get<int>(dimension);
			error::exit_upon_error();
		}
		
		switch(metric) {
			case vector_space_metric::euclidean:
			case vector_space_metric::lorentzian:
				if( field != algebraic_field::real ) {
					BOOST_LOG_SEV(logger, logging::severity_level::error)
						<< "Only real vector spaces can have a euclidean metric.";
					error::exit_upon_error();
				}
				break;
			case vector_space_metric::complex_nondegenerate:
				if( field != algebraic_field::complex ) {
					BOOST_LOG_SEV(logger, logging::severity_level::error)
						<< "Only complex vector spaces can have a complex nondegenerate metric.";
					error::exit_upon_error();
				}
				break;
			case vector_space_metric::unspecified:
				break;
		}
	}
};

[[maybe_unused]] static std::ostream &
operator<<( std::ostream &os, const PQuantum::mathutils::manifold_types::vector_space::algebraic_field &field )
{
	switch( field ) {
		case PQuantum::mathutils::manifold_types::vector_space::algebraic_field::real:
			os << "real";
			break;
		case PQuantum::mathutils::manifold_types::vector_space::algebraic_field::complex:
			os << "complex";
			break;
		case PQuantum::mathutils::manifold_types::vector_space::algebraic_field::complex_grassmann:
			os << "complex grassmann";
			break;
	}
	
	return os;
}

[[maybe_unused]] static std::ostream &
operator<<( std::ostream &os, const PQuantum::mathutils::manifold_types::vector_space::vector_space_metric &metric )
{
	switch( metric ) {
		case PQuantum::mathutils::manifold_types::vector_space::vector_space_metric::unspecified:
			os << "unspecified";
			break;
		case PQuantum::mathutils::manifold_types::vector_space::vector_space_metric::euclidean:
			os << "euclidean";
			break;
		case PQuantum::mathutils::manifold_types::vector_space::vector_space_metric::lorentzian:
			os << "lorentzian";
			break;
		case PQuantum::mathutils::manifold_types::vector_space::vector_space_metric::complex_nondegenerate:
			os << "complex non-degenerate";
			break;
	}
	
	return os;
}
}

using manifold_specification = std::variant<manifold_types::vector_space>;
}
}


#endif //PQUANTUM_MANIFOLD_SPECIFICATION_HPP
