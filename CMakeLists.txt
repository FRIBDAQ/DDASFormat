cmake_minimum_required(VERSION 3.13)
project(DDASFormat VERSION 2.1.0)
 
# Specify the C++ standard:
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
 
# Detect if we're the top-level project or a subproject/ExternalProject:
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(DDASFMT_IS_TOPLEVEL ON)
else()
  set(DDASFMT_IS_TOPLEVEL OFF)
endif()
 
option(DDASFMT_BUILD_TESTS "Build DDASFormat unit tests"    ${DDASFMT_IS_TOPLEVEL})
option(DDASFMT_BUILD_DOCS  "Build DDASFormat documentation" ${DDASFMT_IS_TOPLEVEL})
 
# -----------------------------------------------------------------------
# Library target
# -----------------------------------------------------------------------
set(toplevel_sources
  DDASHit.cpp
  DDASHitUnpacker.cpp
  )
 
set(toplevel_headers
  DDASBitMasks.h
  DDASHit.h
  DDASHitUnpacker.h
  )
 
add_library(DDASFormat SHARED ${toplevel_sources})
 
target_compile_options(DDASFormat PRIVATE -g -O2)
target_link_options(DDASFormat PRIVATE -g)
 
install(FILES ${toplevel_headers} DESTINATION include)
install(TARGETS DDASFormat DESTINATION lib)
 
# -----------------------------------------------------------------------
# Unit tests
# -----------------------------------------------------------------------
if(DDASFMT_BUILD_TESTS)
  enable_testing()
 
  add_executable(ddasfmt_unittests
    TestRunner.cpp
    unpackertests.cpp
    )
 
  target_compile_options(ddasfmt_unittests PRIVATE -g -O2)
  target_link_options(ddasfmt_unittests PRIVATE -g)
  target_link_libraries(ddasfmt_unittests PRIVATE DDASFormat cppunit)
 
  add_test(NAME ddasfmt_tests COMMAND ddasfmt_unittests)
endif()
 
# -----------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------
if(DDASFMT_BUILD_DOCS)
  add_subdirectory(docs)
endif()
