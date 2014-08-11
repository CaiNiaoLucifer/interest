/**
 * @file ExceptionTest.cc
 * @comment
 * implement the test cases of class ExceptionTestCases
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */
#include <boost/test/test_tools.hpp>
#include <string>
#include <iostream>

#include "misc/Exception.h"
#include "ExceptionTest.h"

using namespace std;

void ExceptionTestCases::testException()
{
	try 
	{
		errno = EINTR;
		throw EXCEPTION();
	} catch (Exception &e)
	{
		LOG(LOG_LEVEL_INFO, "catch Exception, %s, %s, %d", 
			e.what(), e.file(), e.lineno())
	}
}

