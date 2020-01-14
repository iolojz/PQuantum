//
// Created by jayz on 14.01.20.
//

#ifndef PQUANTUM_PARSING_QFT_PARSING_CONTEXT_HPP
#define PQUANTUM_PARSING_QFT_PARSING_CONTEXT_HPP

namespace PQuantum::parsing {
struct qft_parsing_context {
	std::function<std::pair<bool, boost::uuids::uuid>( std::string )> field_id_lookup;
	std::function<std::pair<bool, boost::uuids::uuid>( std::string )> parameter_id_lookup;
	std::function<std::pair<bool, boost::uuids::uuid>( std::string )> index_id_lookup_and_generate;
};
}

#endif //PQUANTUM_PARSING_QFT_PARSING_CONTEXT_HPP
