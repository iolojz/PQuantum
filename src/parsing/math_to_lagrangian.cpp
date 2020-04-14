//
// Created by jayz on 13.02.20.
//

#include "math_to_lagrangian.hpp"

#include <boost/variant/static_visitor.hpp>

#include <boost/tti/has_static_member_data.hpp>

using namespace PQuantum;

namespace {
BOOST_TTI_HAS_STATIC_MEMBER_DATA( value )

std::optional<int> string_to_int( const std::string &str ) {
	BOOST_LOG_NAMED_SCOPE( "string_to_int()" );
	logging::severity_logger logger;
	std::size_t num_chars_processed;
	int value;
	
	try {
		value = std::stoi( str, &num_chars_processed );
	} catch( const std::invalid_argument & ) {
		return {};
	} catch( const std::out_of_range & ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Integer overflow upon converting \"" <<
				str << "\" to an int.";
		error::exit_upon_error();
	}
	
	if( num_chars_processed != str.length() ) {
		BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Identifier names may not begin with digits (\"" <<
				str << "\").";
		error::exit_upon_error();
	}
	
	return value;
}

std::variant<int, support::uuid> string_to_index( const std::string &str, parsing::qft_parsing_context &context ) {
	std::optional<int> as_int = string_to_int( str );
	if( as_int ) {
		return *as_int;
	}
	return context.index_id_lookup_and_generate( str );
}

class converter: public boost::static_visitor<model::input_lagrangian_tree> {
	template<class T> using old_node = cxxmath::typesafe_tree_node<T, parsing::math_tree>;
	template<class T> using new_node = cxxmath::typesafe_tree_node<T, model::input_lagrangian_tree>;
	
	parsing::qft_parsing_context &context;
	
	template<class NewContainer, class OldContainer, class ...Unpacked>
	auto self_apply_unpack( OldContainer &&old_container, Unpacked &&...unpacked ) const {
		if constexpr( sizeof...( Unpacked ) == std::tuple_size<std::decay_t<OldContainer>>::value ) {
			return NewContainer{ cxxmath::visit( *this, std::move( unpacked ) )...};
		} else {
			return self_apply_unpack<NewContainer>(
				std::forward<OldContainer>( old_container ),
				std::forward<Unpacked>( unpacked )...,
				std::get<sizeof...( Unpacked )>( std::forward<OldContainer>( old_container ) )
			);
		}
	}
	
	template<class NewContainer, class OldContainer>
	auto convert_constant_size_container( OldContainer &&old_container ) const {
		return self_apply_unpack<NewContainer>( std::forward<OldContainer>( old_container ) );
	}
	
	template<class NewContainer, class OldContainer>
	auto convert_runtime_size_container( OldContainer &&old_container ) const {
		NewContainer new_container;
		if constexpr( std::is_rvalue_reference_v<OldContainer &&> ) {
			std::transform(
				std::make_move_iterator( std::begin( old_container ) ),
				std::make_move_iterator( std::end( old_container ) ),
				std::back_inserter( new_container ),
				[this]( auto &&v ) { return cxxmath::visit( *this, std::forward<decltype( v )>( v ) ); }
			);
		} else {
			std::transform(
				std::begin( old_container ),
				std::end( old_container ),
				std::back_inserter( new_container ),
				[this]( auto &&v ) { return cxxmath::visit( *this, std::forward<decltype( v )>( v ) ); }
			);
		}
		return new_container;
	}
	
	template<class NewContainer, class OldContainer>
	auto convert_container( OldContainer &&old_container ) const {
		if constexpr( has_static_member_data_value<std::tuple_size<OldContainer>, const std::size_t>::value ) {
			static_assert(
				has_static_member_data_value<std::tuple_size<NewContainer>, const std::size_t>::value, "Internal error"
			);
			return convert_constant_size_container<NewContainer>( std::forward<OldContainer>( old_container ) );
		} else {
			static_assert(
				!has_static_member_data_value<std::tuple_size<NewContainer>, const std::size_t>::value, "Internal error"
			);
			return convert_runtime_size_container<NewContainer>( std::forward<OldContainer>( old_container ) );
		}
	}
public:
	converter( parsing::qft_parsing_context &c ) : context{c} {}
	
	template<class OldNodeData> model::input_lagrangian_tree operator()( old_node<OldNodeData> &&node ) const {
		using new_node_type = new_node<OldNodeData>;
		using new_child_container = typename new_node_type::child_container;
		
		return new_node_type{
			std::move( node.data ), convert_container<new_child_container>( std::move( node.children ) )
		};
	}
};

template<> model::input_lagrangian_tree converter::operator()<mathutils::function_call<std::string>>(
	old_node<mathutils::function_call<std::string>> &&fcall_node
) const {
	BOOST_LOG_NAMED_SCOPE( "converter::operator()()" );
	logging::severity_logger logger;
	
	if( fcall_node.data.atom == "\\bar" ) {
		if( std::empty( fcall_node.children ) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "\\bar{} without arguments";
			error::exit_upon_error();
		}
		if( std::size( fcall_node.children ) != 1 ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "\\bar{} with too many arguments";
			error::exit_upon_error();
		}
		
		auto &&child_variant = fcall_node.children.front();
		if( !cxxmath::holds_node<mathutils::atom_with_optional_indices<std::string>>( child_variant ) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "Argument to \\bar{} must be a field";
			error::exit_upon_error();
		}
		auto &&child = cxxmath::get_node<mathutils::atom_with_optional_indices<std::string>>( std::move( child_variant ) );
		if( !context.field_id_lookup( child.data.atom ) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "Argument to \\bar{} must be a field";
			error::exit_upon_error();
		}
		
		std::string barred_name = "\\bar{";
		barred_name += std::move( child.data.atom );
		barred_name.push_back( '}' );

		std::optional<support::uuid> id = context.field_id_lookup( barred_name );
		if( !id ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "The field \"" << child.data.atom << "\" cannot be barred.";
			error::exit_upon_error();
		}
		
		// TODO: currently, there is no way to specify indices of a barred field
		if( !(child.data.indices.lower.empty() && child.data.indices.upper.empty()) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "Only unindexed fields can be barred.";
			error::exit_upon_error();
		}
		
		return model::indexed_field{ *id, {} };
	} else if( fcall_node.data.atom == "\\IndexSum" ) {
		if( std::empty( fcall_node.children ) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "\\IndexSum{} without arguments";
			error::exit_upon_error();
		}
		
		std::vector<support::uuid> indices;
		std::transform(
			std::begin( fcall_node.children ),
			std::end( fcall_node.children ),
			std::back_inserter( indices ),
			[this, &logger] ( const auto &child ) {
				if( cxxmath::holds_node<mathutils::atom_with_optional_indices<std::string>>( child ) == false ) {
					BOOST_LOG_SEV(logger, logging::severity_level::error)
						<< "Arguments to \\IndexSum{} must be indices";
					error::exit_upon_error();
				}
				
				const auto &child_atom =
					cxxmath::get_node<mathutils::atom_with_optional_indices<std::string>>( child ).data;
				if( std::empty( child_atom.indices.upper ) == false ||
					std::empty( child_atom.indices.lower ) == false ) {
					BOOST_LOG_SEV(logger, logging::severity_level::error)
						<< "Indexed index encountered";
					error::exit_upon_error();
				}
				
				auto as_int = string_to_int( child_atom.atom );
				if( as_int ) {
					BOOST_LOG_SEV(logger, logging::severity_level::error)
						<< "Arguments to \\IndexSum{} must be symbolic indices";
					error::exit_upon_error();
				}
				
				return context.index_id_lookup_and_generate( child_atom.atom );
			}
		);
		
		return mathutils::index_sum{
			std::make_move_iterator( std::begin( indices ) ),
			std::make_move_iterator( std::end( indices ) )
		};
	}

	BOOST_LOG_SEV(logger, logging::severity_level::error) << "Unknown function \"" << fcall_node.data.atom << "\"";
	error::exit_upon_error();
}

template<> model::input_lagrangian_tree converter::operator()<mathutils::atom_with_optional_indices<std::string>>(
	old_node<mathutils::atom_with_optional_indices<std::string>> &&indexed_atom_node
) const {
	BOOST_LOG_NAMED_SCOPE( "converter::operator()()" );
	logging::severity_logger logger;
	
	mathutils::index_spec<std::variant<int, support::uuid>> indices;
	
	for( auto &&index : indexed_atom_node.data.indices.lower )
		indices.lower.push_back( string_to_index( index, context ) );
	for( auto &&index : indexed_atom_node.data.indices.upper )
		indices.upper.push_back( string_to_index( index, context ) );
	
	std::optional<support::uuid> id;
	if( (id = context.field_id_lookup( indexed_atom_node.data.atom )) )
		return model::indexed_field{ *id, std::move( indices ) };
	else if( (id = context.parameter_id_lookup( indexed_atom_node.data.atom )) )
		return model::indexed_parameter{ *id, std::move( indices ) };
	else if( indexed_atom_node.data.atom == "\\gamma" ) {
		if( indices.lower.empty() ) {
			if( indices.upper.empty() ) {
				BOOST_LOG_SEV(logger, logging::severity_level::error) << "Gamma matrix without indices";
				error::exit_upon_error();
			}
			if( indices.upper.size() != 1 ) {
				BOOST_LOG_SEV(logger, logging::severity_level::error) << "Gamma matrix with too many indices";
				error::exit_upon_error();
			}

			return mathutils::gamma_matrix{
				mathutils::spacetime_index::index_variance::upper,
				std::move( indices.upper.front() )
			};
		} else if( indices.upper.empty() ) {
			if( indices.lower.size() != 1 ) {
				BOOST_LOG_SEV(logger, logging::severity_level::error) << "Gamma matrix with too many indices";
				error::exit_upon_error();
			}
			
			return mathutils::gamma_matrix{
				{ mathutils::spacetime_index::index_variance::lower, std::move( indices.lower.front() ) }
			};
		}

		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Gamma matrix with too many indices";
		error::exit_upon_error();
	} else if( indexed_atom_node.data.atom == "\\SpacetimeDerivative" ) {
		if( indices.lower.empty() ) {
			if( indices.upper.empty() ) {
				BOOST_LOG_SEV(logger, logging::severity_level::error) << "Spacetime derivative without indices";
				error::exit_upon_error();
			}
			if( indices.upper.size() != 1 ) {
				BOOST_LOG_SEV(logger, logging::severity_level::error) << "Spacetime derivative with too many indices";
				error::exit_upon_error();
			}
			
			return mathutils::spacetime_derivative{
				{ mathutils::spacetime_index::index_variance::upper, std::move( indices.upper.front() ) }
			};
		} else if( indices.upper.empty() ) {
			if( indices.lower.size() != 1 ) {
				BOOST_LOG_SEV(logger, logging::severity_level::error) << "Spacetime derivative with too many indices";
				error::exit_upon_error();
			}
			
			return mathutils::spacetime_derivative{
				{ mathutils::spacetime_index::index_variance::lower, std::move( indices.lower.front() ) }
			};
		}
		
		BOOST_LOG_SEV(logger, logging::severity_level::error) << "Spacetime derivative with too many indices";
		error::exit_upon_error();
	} else if( indexed_atom_node.data.atom == "\\DiracOperator" ) {
		if( !(indices.lower.empty() && indices.upper.empty()) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "Dirac operator with indices";
			error::exit_upon_error();
		}

		return mathutils::dirac_operator{};
	} else if( indexed_atom_node.data.atom == "\\ImaginaryUnit" ) {
		if( !(indices.lower.empty() && indices.upper.empty()) ) {
			BOOST_LOG_SEV(logger, logging::severity_level::error) << "Imaginary unit with indices";
			error::exit_upon_error();
		}

		return mathutils::number{ 0, 1 };
	} else {
		auto as_int = string_to_int( indexed_atom_node.data.atom );
		if( as_int )
			return mathutils::number{ *as_int, 0 };
	}
	
	BOOST_LOG_SEV( logger, logging::severity_level::error ) << "Unrecognized field or parameter identifier \"" <<
		indexed_atom_node.data.atom << "\".";
	error::exit_upon_error();
}
}

namespace PQuantum::parsing {
model::input_lagrangian_tree math_to_input_lagrangian( math_tree &&mtree, qft_parsing_context &&qft_context ) {
	BOOST_LOG_NAMED_SCOPE( "parsing::math_to_lagrangian()" );
	return cxxmath::visit( converter{ qft_context }, std::move( mtree ) );
}
}
