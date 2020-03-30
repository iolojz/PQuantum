//
// Created by jayz on 16.01.20.
//

#ifndef PQUANTUM_MATHUTILS_ARITHMETIC_HPP
#define PQUANTUM_MATHUTILS_ARITHMETIC_HPP

#include <boost/hana.hpp>
#include <cxxmath/cxxmath.hpp>

#include <ostream>

namespace PQuantum::mathutils {
struct power {};
struct product {};
struct quotient {};
struct sum {};
struct difference {};
struct negation {};

static constexpr auto ops_by_precedence = boost::hana::tuple_t<power, product, quotient, sum, difference>;

static constexpr auto arithmetic_arity_map = boost::hana::make_map(
	boost::hana::make_pair( boost::hana::type_c<power>, boost::hana::int_c<2> ),
	boost::hana::make_pair( boost::hana::type_c<product>, cxxmath::runtime_arity ),
	boost::hana::make_pair( boost::hana::type_c<quotient>, boost::hana::int_c<2> ),
	boost::hana::make_pair( boost::hana::type_c<sum>, cxxmath::runtime_arity ),
	boost::hana::make_pair( boost::hana::type_c<difference>, boost::hana::int_c<2> ),
	boost::hana::make_pair( boost::hana::type_c<negation>, boost::hana::int_c<1> )
);

#ifdef PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR
#error "PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR is already defined"
#endif
#define PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( op_name, op_str ) \
template<class TreeNode> \
std::ostream &operator<<( std::ostream &os, op_name ) { \
    return os << op_str; \
}

PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( power, "^" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( product, "*" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( quotient, "/" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( sum, "+" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( difference, "-" )
PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR( negation, "-" )

#undef PQUANTUM_DEFINE_ARITHMETIC_NODE_OSTREAM_OPERATOR
}

#endif //PQUANTUM_MATHUTILS_ARITHMETIC_HPP
