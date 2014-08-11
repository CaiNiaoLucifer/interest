#!/usr/local/bin/python

##
# create a empty project in current directory
# with autoconf tools
#
import os
import sys
import re

## declaring of functions
def mkdir(dir):
	"""make a directory if not exists"""
	if not os.path.exists(dir):
		os.makedirs(dir)


# check the input arguments
if len(sys.argv) < 4:
	print 'Usage: createEmpytProject ProjectName Version Email.'
	exit(1)

project = sys.argv[1]
version = sys.argv[2]
email = sys.argv[3]

# make the directories of project
mkdir(project)
os.chdir(project)
mkdir('include')
mkdir('include/misc')
mkdir('src')
mkdir('src/misc')
mkdir('test')
mkdir('scripts')
mkdir('build')
mkdir('doc')
mkdir('m4')

# generate configure.ac
print("Generating configure.ac...")
os.system('autoscan')
file = open("configure.scan")
ac = file.read()
file.close()

append = "AC_INIT([" + project + "], [" + version + "], [" + email + "])\n\
AC_CONFIG_SRCDIR([])\n\
AM_INIT_AUTOMAKE(" + project + ", " + version + ")\n\
AC_CONFIG_HEADERS([config.h])\n\
AC_CONFIG_MACRO_DIR([m4])"
pattern = "AC_INIT\(\[FULL-PACKAGE-NAME\], \[VERSION\], \[BUG-REPORT-ADDRESS\]\)"
ac = re.sub(pattern, append, ac)

append = "# Checks for programs.\n\
AC_PROG_CC(clang llvm-gcc gcc)\n\
AC_PROG_CXX(clang++ llvm-g++ g++)\n\
AC_PROG_CPP\n\
AC_PROG_AWK\n\
AC_PROG_INSTALL\n\
AC_PROG_LN_S\n\
AC_PROG_MAKE_SET\n\
LT_INIT\n\
AC_PROG_LIBTOOL\n\
AC_ARG_ENABLE(debug, [  --enable-debug\tEnable DEBUG output. ],\n\
\t[ CXXFLAGS=\"-O0 -DDEBUG -Wall -Werror\" ],\n\
\t[ CXXFLAGS=\"-O3 -Wall -Werror\" ])"
ac = re.sub("# Checks for programs.", append, ac)

append = "AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])\n\
AC_OUTPUT"
ac = re.sub("AC_OUTPUT", append, ac)

file = open("configure.ac", 'w')
file.write(ac)
file.close()
os.system('rm autoscan*.log')
os.system('rm configure.scan')
del file
del append

# generate automake.am
print("Generating Makefile.am...")
am = open('Makefile.am', 'w')
am.write('ACLOCAL_AMFLAGS = -I m4\nSUBDIRS = src test\nEXTRA_DIST = include doc')
am.close()

os.chdir('test')
am = open('Makefile.am', 'w')
am.write('INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src\n' +
		 'include_HEADERS = suites.h LogTest.h ExceptionTest.h\n' +
		 'noinst_PROGRAMS = ../bin/test\n' +
		 '___bin_test_SOURCES = test.cc LogTest.cc ExceptionTest.cc\n' +
		 '___bin_test_LDADD = ../src/lib' + project + '.la\n' +
		 '___bin_test_LDFLAGS = -L/usr/local/lib -lboost_unit_test_framework')
am.close()
os.system('cp ~/reposit/test.cc .')
os.system('cp ~/reposit/suites.h .')
os.system('cp ~/reposit/LogTest.h .')
os.system('cp ~/reposit/LogTest.cc .')
os.system('cp ~/reposit/ExceptionTest.h .')
os.system('cp ~/reposit/ExceptionTest.cc .')

os.chdir('../include/misc')
os.system('cp ~/reposit/Log.h .')
os.system('cp ~/reposit/Exception.h .')
os.chdir('../../src/misc')
os.system('cp ~/reposit/Log.cc .')

os.chdir('..')
am = open('Makefile.am', 'w')
am.write('INCLUDES = -I$(top_srcdir)/include\n' +
		 'include_HEADERS = \n' +
		 '\n#\n' +
		 '# lib' + project + '.la\n' +
		 '#\n' +
		 'lib_LTLIBRARIES = lib' + project + '.la\n' +
		 'lib' + project + '_la_SOURCES = ./misc/Log.cc')
am.close()
os.chdir('..')

# copy integrated test framework
print('Copying integrated test framework...')
os.chdir('scripts')
os.system('cp ~/reposit/itframe.py .')
os.system('cp ~/reposit/test.py .')
f = open('test.py', 'r')
kk = f.read()
f.close()
kkk = re.sub('\?\?\?', project, kk)
f = open('test.py', 'w')
f.write(str(kkk))
f.close()
os.chdir('..')
del f
del kk
del kkk

# copy document templates
print('Copying document templates...')
os.chdir('doc')
os.system('cp ~/reposit/requirements.dot .')
os.system('cp ~/reposit/architecture.dot .')
os.chdir('..')

# append files
print('Appending some files...')
os.system('touch NEWS')
os.system('touch README')
os.system('touch AUTHORS')
os.system('touch COPYING')
os.system('touch INSTALL')
os.system('touch ChangeLog')

# configure
print('Configure...')
os.system('libtoolize')
os.system('aclocal')
os.system('autoheader')
os.system('autoconf')
os.system('automake --add-missing')

# build
print('Building...')
os.chdir('build')
os.system('../configure --enable-debug')
os.system('make')

