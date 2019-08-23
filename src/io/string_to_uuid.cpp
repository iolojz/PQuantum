//
// Created by jayz on 22.08.19.
//

#include "string_to_uuid.hpp"

#include <boost/uuid/uuid_generators.hpp>

namespace PQuantum
{
namespace io
{
boost::uuids::uuid string_to_uuid::operator()( const std::string &str ) const
{
	auto it = uid_map.find( str );
	if( it != uid_map.end() )
		return it->second;
	
	return uid_map.insert( { str, boost::uuids::random_generator()() } ).first->second;
}
}
}