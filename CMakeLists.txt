cmake_minimum_required (VERSION 2.6)
project (LogScanner C CXX)

SET_DIRECTORY_PROPERTIES(PROPERTIES 
   ADDITIONAL_MAKE_CLEAN_FILES "cmake_install.cmake;CMakeCache.txt;Makefile") 

set (LogScanner_VERSION_MAJOR 0)
set (LogScanner_VERSION_MINOR 0)
set (LogScanner_VERSION_PATCH 0)

set (CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set (CMAKE_CXX_FLAGS "-std=c++11 -pthread")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package (Threads REQUIRED)
set (LIBS ${LIBS} ${Threads_LIBRARIES})

find_package (Glog REQUIRED)
set (LIBS ${LIBS} ${GLOG_LIBRARIES})

# TODO: Write cmake modules for the following:
set (LIBS ${LIBS} gflags)
set (LIBS ${LIBS} unwind)

add_library (filescanner STATIC src/FileScanner.cpp src/FileScanner.h)
add_executable (logscanner src/logscanner.cpp)

target_link_libraries (logscanner filescanner ${LIBS})
