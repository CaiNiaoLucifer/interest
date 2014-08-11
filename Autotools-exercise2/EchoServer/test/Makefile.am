INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src
include_HEADERS = suites.h LogTest.h ExceptionTest.h
noinst_PROGRAMS = ../bin/test
___bin_test_SOURCES = test.cc LogTest.cc ExceptionTest.cc
___bin_test_LDADD = ../src/libEchoServer.la
___bin_test_LDFLAGS = -L/usr/local/lib -lboost_unit_test_framework