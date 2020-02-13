//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_QFT_PARSING_CONTEXT_HPP
#define PQUANTUM_PARSING_QFT_PARSING_CONTEXT_HPP

#include <functional>

#include "support/uuid.hpp"

namespace PQuantum::parsing {
struct field_id_lookup_tag {};
struct parameter_id_lookup_tag {};
struct index_id_lookup_and_generate_tag {};

struct qft_parsing_context {
	std::function<std::optional<support::uuid>( std::string )> field_id_lookup;
	std::function<std::optional<support::uuid>( std::string )> parameter_id_lookup;
	std::function<support::uuid( std::string )> index_id_lookup_and_generate;
};
}

#endif //PQUANTUM_PARSING_QFT_PARSING_CONTEXT_HPP
