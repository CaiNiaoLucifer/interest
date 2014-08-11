/**
 * @file Exception.h
 * @comment
 * define the Exception here
 * 
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>
#include <string.h>
#include <errno.h>
#include "Log.h"

#define EXCEPTION()	Exception(errno, __FILE__, __LINE__)

/**
 * @class Exception
 * @comment
 * Exception extend std::exception
 */
class Exception : public std::exception
{
  protected:
	int errno_;
	const char *position_;
	int line_;

  public:
	Exception(int e, const char *f, int l) : errno_(e), position_(f), line_(l)
	{ LOG(LOG_LEVEL_ERROR, "%s, %s, %d\n", strerror(errno_), position_, line_) }
	~Exception() throw() {}

	int lineno() { return line_; }
	const char* file() { return position_; }
	const char* what() const throw() { return strerror(errno_); }
};

#endif // _EXCEPTION_H_
