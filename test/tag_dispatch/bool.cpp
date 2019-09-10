//
// Created by jayz on 04.09.19.
//

#define BOOST_TEST_MODULE Tag Dispatch - bool

#include <boost/test/included/unit_test.hpp>

#include "tag_dispatch/tag_dispatch.hpp"

BOOST_AUTO_TEST_CASE(bool) {
	using namespace tag_dispatch;
	
	static constexpr bool yes = true;
	static constexpr bool no = false;
	
	BOOST_TEST(not_<>(equal<>(yes, no)));
	
	BOOST_TEST(and_<>(yes, yes));
	BOOST_TEST(not_<>(and_<>(yes, no)));
	BOOST_TEST(not_<>(and_<>(no, yes)));
	BOOST_TEST(not_<>(and_<>(no, no)));
	
	BOOST_TEST(or_<>(yes, yes));
	BOOST_TEST(or_<>(yes, no));
	BOOST_TEST(or_<>(no, yes));
	BOOST_TEST(not_<>(or_<>(no, no)));
	
	BOOST_TEST(equal<>(yes, yes));
	BOOST_TEST(not_<>(equal<>(yes, no)));
	BOOST_TEST(not_<>(equal<>(no, yes)));
	BOOST_TEST(equal<>(no, no));
}

BOOST_AUTO_TEST_CASE(std_bool_constant) {
	using namespace tag_dispatch;
	
	static constexpr std::true_type yes;
	static constexpr std::false_type no;
	
	static_assert(not_<>(equal<>(yes, no)).value);
	
	static_assert(and_<>(yes, yes).value);
	static_assert(not_<>(and_<>(yes, no)).value);
	static_assert(not_<>(and_<>(no, yes)).value);
	static_assert(not_<>(and_<>(no, no)).value);
	
	static_assert(or_<>(yes, yes).value);
	static_assert(or_<>(yes, no).value);
	static_assert(or_<>(no, yes).value);
	static_assert(not_<>(or_<>(no, no)).value);
	
	static_assert(equal<>(yes, yes).value);
	static_assert(not_<>(equal<>(yes, no)).value);
	static_assert(not_<>(equal<>(no, yes)).value);
	static_assert(equal<>(no, no).value);
}
