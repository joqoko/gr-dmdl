# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pwa/Source/gr-dmdl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pwa/Source/gr-dmdl/build

# Utility rule file for dmdl_swig_swig_doc.

# Include the progress variables for this target.
include swig/CMakeFiles/dmdl_swig_swig_doc.dir/progress.make

swig/CMakeFiles/dmdl_swig_swig_doc: swig/dmdl_swig_doc.i


swig/dmdl_swig_doc.i: swig/dmdl_swig_doc_swig_docs/xml/index.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pwa/Source/gr-dmdl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating python docstrings for dmdl_swig_doc"
	cd /home/pwa/Source/gr-dmdl/docs/doxygen && /usr/bin/python2 -B /home/pwa/Source/gr-dmdl/docs/doxygen/swig_doc.py /home/pwa/Source/gr-dmdl/build/swig/dmdl_swig_doc_swig_docs/xml /home/pwa/Source/gr-dmdl/build/swig/dmdl_swig_doc.i

swig/dmdl_swig_doc_swig_docs/xml/index.xml: swig/_dmdl_swig_doc_tag
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pwa/Source/gr-dmdl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating doxygen xml for dmdl_swig_doc docs"
	cd /home/pwa/Source/gr-dmdl/build/swig && ./_dmdl_swig_doc_tag
	cd /home/pwa/Source/gr-dmdl/build/swig && /usr/bin/doxygen /home/pwa/Source/gr-dmdl/build/swig/dmdl_swig_doc_swig_docs/Doxyfile

dmdl_swig_swig_doc: swig/CMakeFiles/dmdl_swig_swig_doc
dmdl_swig_swig_doc: swig/dmdl_swig_doc.i
dmdl_swig_swig_doc: swig/dmdl_swig_doc_swig_docs/xml/index.xml
dmdl_swig_swig_doc: swig/CMakeFiles/dmdl_swig_swig_doc.dir/build.make

.PHONY : dmdl_swig_swig_doc

# Rule to build all files generated by this target.
swig/CMakeFiles/dmdl_swig_swig_doc.dir/build: dmdl_swig_swig_doc

.PHONY : swig/CMakeFiles/dmdl_swig_swig_doc.dir/build

swig/CMakeFiles/dmdl_swig_swig_doc.dir/clean:
	cd /home/pwa/Source/gr-dmdl/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/dmdl_swig_swig_doc.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/dmdl_swig_swig_doc.dir/clean

swig/CMakeFiles/dmdl_swig_swig_doc.dir/depend:
	cd /home/pwa/Source/gr-dmdl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pwa/Source/gr-dmdl /home/pwa/Source/gr-dmdl/swig /home/pwa/Source/gr-dmdl/build /home/pwa/Source/gr-dmdl/build/swig /home/pwa/Source/gr-dmdl/build/swig/CMakeFiles/dmdl_swig_swig_doc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/dmdl_swig_swig_doc.dir/depend

