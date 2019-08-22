//
// Created by jayz on 19.08.19.
//

#include "manifold_specification.hpp"

namespace PQuantum
{
namespace mathutils
{
namespace manifold_types
{
std::ostream &
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

std::ostream &
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
}
}
