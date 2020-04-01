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
	constexpr uuid( const uuid & ) = default;
	constexpr uuid( uuid && ) = default;
	
	constexpr uuid &operator=( const uuid & ) = default;
	constexpr uuid &operator=( uuid && ) = default;
	
	uuid( boost::uuids::random_generator &gen )
	: data_( gen() ) {}
	
	bool operator<( const uuid &id ) const {
		return data_ < id.data_;
	}
	
	bool operator<=( const uuid &id ) const {
		return data_ <= id.data_;
	}
	
	constexpr bool operator==( const uuid &id ) const {
		for( std::size_t i = 0; i != boost::uuids::uuid::static_size(); i++ ) {
			if( data_.data[i] != id.data_.data[i] )
				return false;
		}
		return true;
	}
	
	constexpr bool operator!=( const uuid &id ) const {
		return !(*this == id);
	}
	
	bool operator>=( const uuid &id ) const {
		return data_ >= id.data_;
	}
	
	bool operator>( const uuid &id ) const {
		return data_ > id.data_;
	}
	
	constexpr boost::uuids::uuid data( void ) const {
		return data_;
	}
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const uuid &id ) {
	return os << id.data();
}
}

#endif //PQUANTUM_SUPPORT_UUID_HPP
