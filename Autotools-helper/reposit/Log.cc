/**
 * @file Log.c
 * @comment
 * define the global variable logLevel here
 * 
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */

#include "misc/Log.h"

int logLevel = LOG_LEVEL_INFO;

class Logger
{
public:
	Logger()
	{ openlog(NULL, LOG_PID, LOG_USER); }
	~Logger()
	{ closelog(); }
};

// make log is open and closed
static Logger logger;

