A good helper for using Autotools.

The helper can help you create empty project using autotools and you just need
to modify a few files that you can build your own project.

####How to use:

1. Make sure that reposit directory is under the $(HOME)
2. run createEmptyProject.py script with three parameters
- project name
- version
- bug email
3. Now you can find the new produced project directory eg.EchoServer and you
   need to put your source files in the src dir and you header files in the
   include dir
4.  You should modify some files. In the next, EchoServer means the project top_dir
- $(top_dir)/configure.ac:if you want to add your own compile options,modify it.
- $(top_dir)/Makefile.am: if you want to add or delete sundirs, modify it.(the
  subdirs you write control whether to compile the source files in them)
- $(top_dir)/src/Makefile.am: you need to write in this file that where you put your
  executable file and the source files requested to produce the executable file
  and the link infomation and your own header files' path
5. return to the $(top_dir) run autoreconf
6. Enter the $(top_dir)/build run ../configure then make
