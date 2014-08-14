#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([EchoServer], [1.0], [649412424@qq.com])
AC_CONFIG_SRCDIR([])
AM_INIT_AUTOMAKE(EchoServer, 1.0)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC(clang llvm-gcc gcc)
AC_PROG_CXX(clang++ llvm-g++ g++)
AC_PROG_CPP
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT
AC_PROG_LIBTOOL
AC_ARG_ENABLE(debug, [  --enable-debug	Enable DEBUG output. ],
	[ CXXFLAGS="-O0 -DDEBUG -Wall -Werror -march=native -rdynamic -std=c++11"],
	[ CXXFLAGS="-O3 -Wall -Werror -march=native -rdynamic -std=c++11" ])

# Checks for libraries.

AC_CHECK_LIB([rt], [clock_gettime], [], [AC_MSG_FAILURE([rt liberary not found])])
AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_FAILURE([pthread liberary not found])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
