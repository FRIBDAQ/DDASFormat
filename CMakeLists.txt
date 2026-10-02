cmake_minimum_required(VERSION 3.13)

project(DDASFormat VERSION 2.1.002)

# Detect whether we are the top-level project or being built as a subproject
# (via add_subdirectory / FetchContent). PROJECT_IS_TOP_LEVEL is built in on
# CMake >= 3.21; provide a fallback for the 3.13 minimum.

if(NOT DEFINED PROJECT_IS_TOP_LEVEL)
  if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PROJECT_IS_TOP_LEVEL ON)
  else()
    set(PROJECT_IS_TOP_LEVEL OFF)
  endif()
endif()

find_package(Doxygen)

# Documentation is built by default only when this is the top-level project and
# Doxygen is available. A parent project can still force it on with -DBUILD_DOCS=ON.

set(_ddas_docs_default OFF)
if(DOXYGEN_FOUND AND PROJECT_IS_TOP_LEVEL)
  set(_ddas_docs_default ON)
endif()
option(BUILD_DOCS "Build documentation (requires Doxygen)" ${_ddas_docs_default})

# Unit tests are built by default only for the top-level project, so consumers
# that pull DDASFormat in as a subproject are not forced to provide CppUnit.

option(DDASFORMAT_BUILD_TESTS "Build unit tests (requires CppUnit)"
  ${PROJECT_IS_TOP_LEVEL})

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
 
set(ddasfmt_headers
  DDASBitMasks.h
  DDASHit.h
  DDASHitUnpacker.h
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

if(DDASFORMAT_BUILD_TESTS)

  enable_testing()

  add_executable(unittests
    TestRunner.cpp
    unpackertests.cpp
    )

  target_compile_options(unittests PRIVATE -Wall -Wextra)
  target_link_libraries(unittests DDASFormat cppunit)

  add_test(NAME DDASHitTests COMMAND unittests)

endif()
