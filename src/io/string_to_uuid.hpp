//
// Created by jayz on 22.08.19.
//

#ifndef PQUANTUM_STRING_TO_UUID_HPP
#define PQUANTUM_STRING_TO_UUID_HPP

#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>

namespace PQuantum
{
namespace io
{
class string_to_uuid
{
	mutable std::map<std::string, boost::uuids::uuid> uid_map;
public:
	boost::uuids::uuid operator()( const std::string &str ) const;
};
}
}


#endif //PQUANTUM_STRING_TO_UUID_HPP
