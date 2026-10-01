cmake_minimum_required(VERSION 3.13)

project(DDASFormat VERSION 2.1.002)

find_package(Doxygen)
option(BUILD_DOCS "Build documentation (requires Doxygen)" ${DOXYGEN_FOUND})

# Specify the C++ standard:

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Default to an optimized build with debug symbols (-O2 -g) when the user has
# not chosen a build type. The per-type optimization/debug flags are supplied
# by CMake, so a user-specified build level (e.g. -DCMAKE_BUILD_TYPE=Debug or
# =Release) cleanly overrides this default.

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
    "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  Debug Release RelWithDebInfo MinSizeRel)

# Add subdirectories for e.g., documentation:

add_subdirectory(docs)

# Set the top-level library target:

set (toplevel_sources
  DDASHit.cpp DDASHitUnpacker.cpp
  )

set (toplevel_headers
  DDASBitMasks.h DDASHit.h DDASHitUnpacker.h
  )

add_library(DDASFormat SHARED ${toplevel_sources})

# Optimization and debug flags come from CMAKE_BUILD_TYPE (see above); only
# the warning flags are specified explicitly here. Remove -Wall -Wextra if an
# unwarned build is preferred.

target_compile_options(DDASFormat PRIVATE -Wall -Wextra)

# Set install path for headers and libs:

install (
  FILES ${toplevel_headers} DESTINATION include
  )
install (TARGETS DDASFormat DESTINATION lib)

##
# Unit tests
#

enable_testing()

add_executable(unittests
  TestRunner.cpp
  unpackertests.cpp
  )

target_compile_options(unittests PRIVATE -Wall -Wextra)
target_link_libraries(unittests DDASFormat cppunit)

add_test(NAME DDASHitTests COMMAND unittests)
