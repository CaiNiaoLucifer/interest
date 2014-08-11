/**
 * @file Log.h
 * @comment
 * declare the Log function here
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */
 
#ifndef _XCOM_LOG_H_
#define _XCOM_LOG_H_
	 
#include <syslog.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define LOG_LEVEL_INFO		LOG_INFO
#define LOG_LEVEL_WARNING	LOG_WARNING
#define LOG_LEVEL_ERROR		LOG_ERR

#if defined(__cplusplus)
extern "C" {
#endif

extern int logLevel;

#define LogError()										\
	LOG(LOG_LEVEL_ERROR, "%s, %s, %d\n", 				\
			strerror(errno), __FILE__, __LINE__)
	 
#if defined(DEBUG)
	 
#define LOG(l, fm, ...)									\
	if (l <= logLevel) { printf(fm, ##__VA_ARGS__); }
	 
#else
	 
#define LOG(l, fm, ...)									\
	if (l <= logLevel) { syslog(l, fm, ##__VA_ARGS__); }

#endif /* DEBUG */

#if defined(__cplusplus)
}
#endif
	 
#endif /* _XCOM_LOG_H_ */

