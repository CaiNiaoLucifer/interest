/**
 * @file test.cc
 * @description
 * 	unit test suites for mds project.
 * 
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */

#include <boost/test/included/unit_test.hpp>
using boost::unit_test::test_suite;

#include "suites.h"

test_suite *init_unit_test_suite(int argc, char *argv[])
{
	// test suites for class Agent
	test_suite *suites = BOOST_TEST_SUITE("Unit test for project proactor");

	// organize all test suites
	suites->add(new LogTestSuite());
	suites->add(new ExceptionTestSuite());

	return suites;
}


