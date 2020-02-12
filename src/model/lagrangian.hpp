//
// Created by jayz on 06.08.19.
//

#ifndef PQUANTUM_MODEL_LAGRANGIAN_HPP
#define PQUANTUM_MODEL_LAGRANGIAN_HPP


namespace PQuantum::model {

struct atom_with_optional_indices {
	decltype(atom::name) name;
	index_spec indices;
};
}

#endif //PQUANTUM_MODEL_LAGRANGIAN_HPP
