//
// Created by jayz on 13.02.20.
//

#ifndef PQUANTUM_SUPPORT_UUID_HPP
#define PQUANTUM_SUPPORT_UUID_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

namespace PQuantum::support {
class uuid {
	boost::uuids::uuid data_;
public:
	uuid( void ) = delete;
	uuid( const uuid & ) = default;
	uuid( uuid && ) = default;
	
	uuid &operator=( const uuid & ) = default;
	uuid &operator=( uuid && ) = default;
	
	uuid( boost::uuids::random_generator &gen )
		: data_( gen() ) {}
	
	bool operator<( const uuid &id ) const {
		return data_ < id.data_;
	}
	
	bool operator<=( const uuid &id ) const {
		return data_ <= id.data_;
	}
	
	bool operator==( const uuid &id ) const {
		return data_ == id.data_;
	}
	
	bool operator!=( const uuid &id ) const {
		return data_ != id.data_;
	}
	
	bool operator>=( const uuid &id ) const {
		return data_ >= id.data_;
	}
	
	bool operator>( const uuid &id ) const {
		return data_ > id.data_;
	}
	
	boost::uuids::uuid data( void ) const {
		return data_;
	}
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const uuid &id ) {
	return os << id.data();
}
}

#endif //PQUANTUM_SUPPORT_UUID_HPP
