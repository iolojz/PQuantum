//
// Created by jayz on 11.01.20.
//

#ifndef PQUANTUM_VARIABLE_HPP
#define PQUANTUM_VARIABLE_HPP

namespace PQuantum::mathutils {
struct variable_id {
	boost::uuids::uuid id;
};
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::variable_id, id )

#endif //PQUANTUM_VARIABLE_HPP
