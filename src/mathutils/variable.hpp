//
// Created by jayz on 11.01.20.
//

#ifndef PQUANTUM_VARIABLE_HPP
#define PQUANTUM_VARIABLE_HPP

namespace PQuantum::mathutils {
struct variable_id {
	boost::uuids::uuid id;
};

bool operator<( const variable_id &id1, const variable_id &id2 ) {
	return id1.id < id2.id;
}
}

BOOST_FUSION_ADAPT_STRUCT( PQuantum::mathutils::variable_id, id )

#endif //PQUANTUM_VARIABLE_HPP
