//
// Created by jayz on 27.07.19.
//

#ifndef PQUANTUM_LOGGING_HPP
#define PQUANTUM_LOGGING_HPP

#ifndef BOOST_LOG_DYN_LINK
#define BOOST_LOG_DYN_LINK
#endif

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp> // For BOOST_LOG_STREAM_WITH_PARAMS() used by BOOST_LOG_SEV()
#include <boost/log/attributes/named_scope.hpp> // For BOOST_LOG_NAMED_SCOPE() for easy scope specification

namespace PQuantum::logging
{
void setup_logging_facilities( void );

namespace severity_level
{
/** FIXME: Unfortunately, this needs to be an old-school enum to get
 * things to work with boost log...
 **/
enum severity_level {
	user_info, //< output to std::cout
	trace, //< output to std::cout
	debug, //< output to std::cerr
	warning, //< output to std::cerr
	error, //< output to std::cerr
	internal_error //< output to std::cerr
};
}

using severity_logger = boost::log::sources::severity_logger<severity_level::severity_level>;
}

#endif //PQUANTUM_LOGGING_HPP
