cmake_minimum_required(VERSION 3.13)

project(DDASFormat VERSION 1.0.006)

option(BUILD_DOCS "Build documentation" ON)

# Specify the C++ standard:

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# Compile flags for the library:

target_compile_options(DDASFormat PRIVATE -g -O2)
target_link_options(DDASFormat PRIVATE -g)

# Set install path for headers and libs:

install (
  FILES ${toplevel_headers} DESTINATION include
  )
install (TARGETS DDASFormat DESTINATION lib)

##
# Unit tests
#

enable_testing()

add_executable(unpackertests
  TestRunner.cpp
  UnpackerTests.cpp
  )

target_compile_options(unpackertests PRIVATE -g -O2)
target_link_options(unpackertests PRIVATE -g)

target_link_libraries(unpackertests DDASFormat cppunit)

add_test(NAME unpacker COMMAND unpackertests)
