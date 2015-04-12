cmake_minimum_required (VERSION 2.6)
project (LogScanner)


SET_DIRECTORY_PROPERTIES(PROPERTIES 
   ADDITIONAL_MAKE_CLEAN_FILES "CMakeFiles;cmake_install.cmake;CMakeCache.txt;Makefile") 

set (LogScanner_VERSION_MAJOR 1)
set (LogScanner_VERSION_MINOR 0)

add_executable(logscanner src/logscanner.cpp)
