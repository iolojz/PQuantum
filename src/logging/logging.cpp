//
// Created by jayz on 28.07.19.
//

#include "logging.hpp"

#include <boost/core/null_deleter.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/expressions/formatters.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <iostream>

BOOST_LOG_ATTRIBUTE_KEYWORD( object_tag, "object", std::string )
BOOST_LOG_ATTRIBUTE_KEYWORD( scope_tag, "scope", boost::log::attributes::named_scope::value_type )
BOOST_LOG_ATTRIBUTE_KEYWORD(severity_tag, "Severity", PQuantum::logging::severity_level::severity_level)

namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace PQuantum::logging {
namespace severity_level {
static std::ostream &operator<<(std::ostream &os, severity_level slevel) {
	switch(slevel) {
		case user_info:
			return os << "user_info";
			break;
		case trace:
			return os << "trace";
			break;
		case debug:
			return os << "debug";
			break;
		case warning:
			return os << "warning";
			break;
		case error:
			return os << "error";
			break;
		case internal_error:
			return os << "internal error";
			break;
	}
	
	return os;
}
}

void setup_logging_facilities( void )
{
	boost::log::core::get()->add_global_attribute( "scope", boost::log::attributes::named_scope());
	
	boost::log::formatter formatter = expr::stream
	<< expr::if_( expr::has_attr( object_tag ))[expr::stream << object_tag
															 << expr::if_( expr::has_attr( scope_tag ))[expr::stream
															 << "::"]]
	<< expr::if_( expr::has_attr( scope_tag ))[expr::stream
	<< expr::attr<boost::log::attributes::named_scope::value_type>( "scope" )//scope_tag
	] << " <" << severity_tag << ">: " << expr::smessage;
	
	using text_sink = sinks::synchronous_sink<sinks::text_ostream_backend>;
	auto cout_sink = boost::make_shared<text_sink>();
	auto clog_sink = boost::make_shared<text_sink>();
	
	cout_sink->locked_backend()->add_stream( boost::shared_ptr<std::ostream>( &std::cout, boost::null_deleter()));
	clog_sink->locked_backend()->add_stream( boost::shared_ptr<std::ostream>( &std::clog, boost::null_deleter()));
	
	cout_sink->set_formatter( formatter );
	clog_sink->set_formatter( formatter );
	
	cout_sink->locked_backend()->auto_flush( true );
	clog_sink->locked_backend()->auto_flush( true );
	
	cout_sink->set_filter( severity_tag <= severity_level::trace );
	clog_sink->set_filter( severity_tag >= severity_level::debug );
	
	boost::log::core::get()->add_sink( cout_sink );
	boost::log::core::get()->add_sink( clog_sink );
}
}
