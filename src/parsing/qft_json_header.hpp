//
// Created by jayz on 01.08.19.
//

#ifndef PQUANTUM_QFT_JSON_HEADER_HPP
#define PQUANTUM_QFT_JSON_HEADER_HPP

#include <string>

namespace PQuantum::parsing {
struct json_schema_header {
	std::string name;
	int version_major;
	int version_minor;
};

[[maybe_unused]] static std::ostream &operator<<( std::ostream &os, const json_schema_header &header ) {
	return os << header.name << ", version = " << header.version_major << "." << header.version_minor;
}
}

#endif //PQUANTUM_QFT_JSON_HEADER_HPP
