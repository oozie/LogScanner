cmake_minimum_required (VERSION 2.6)
project (LogScanner)


SET_DIRECTORY_PROPERTIES(PROPERTIES 
   ADDITIONAL_MAKE_CLEAN_FILES "CMakeFiles;cmake_install.cmake;CMakeCache.txt;Makefile") 

set (LogScanner_VERSION_MAJOR 1)
set (LogScanner_VERSION_MINOR 0)

set (CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set (BUILD_SHARED_LIBRARIES OFF)
set (CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(logscanner src/logscanner.cpp)
