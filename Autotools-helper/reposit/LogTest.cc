/**
 * @file LogTest.cc
 * @comment
 * implement the test cases declared in LogTest.h
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */
 
#include <boost/test/test_tools.hpp>
#include <string>
#include <iostream>
	  
#include "LogTest.h"
#include "misc/Log.h"

void LogTestCases::testLog()
{
	LOG(LOG_LEVEL_INFO, "you will see this message in /var/log/user.log file, \
if you configure without debug.\n");

	BOOST_CHECK(true);
}

