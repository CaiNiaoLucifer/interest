MUDUO_DIRECTORY = $(HOME)/build/debug-install
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
INCLUDES = -I$(top_srcdir)/include -I$(HOME)/build/debug-install/include
include_HEADERS =

#
# libEchoServer.la
#
bin_PROGRAMS = $(top_builddir)/bin/EchoServer
__top_builddir__bin_EchoServer_SOURCES = ./misc/Log.cc codec.cc main.cc EchoMessage.pb.cc echo.cc
__top_builddir__bin_EchoServer_LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net_cpp11 -lmuduo_base_cpp11 -lpthread -lrt -lprotobuf \
										 -lboost_program_options
