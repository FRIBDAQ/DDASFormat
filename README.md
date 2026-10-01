# DDASFormat

## Introduction

The transformation of binary event data from a particular set of acquisition electronics into a more useful representation is colloquially referred to as "unpacking" and the code that performs this task are therefore called "unpackers." This project provides the DDASFormat library and unpacker which can be used in programs which process DDAS data e.g., the NSCLDAQ's `ddasdumper` or `SpecTcl`. The DDASFormat library supports unpacking binary data into class objects that encapsulate the information from a single-channel hit with a defined interface. The DDASFormat library and unpacker can be used as-is on systems where NSCLDAQ is not installed as it does not depend on any FRIB software packages.

The DDASFormat library consists of the following:
* `ddasfmt::DDASHit` - A class which encapsulates the information emitted by a single Pixie-16 digitizer channel.
* `ddasfmt::DDASHitUnpacker` - Unpacker for DDAS data recorded by NSCLDAQ. Data is unpacked into DDASHits.
* `DDASBitMasks.h` - Header containing masks used to extract information from specific parts of XIA Pixie list-mode data. The bitmasks are also available in the ddasfmt namespace.

## Requirements

The minimum software requirements are listed below. The code has been built and tested extensively on Debian 10 (buster), 11 (bullseye), and 12 (bookworm) systems at FRIB.

* A C++ compiler with support for the C++11 standard (e.g., GCC 4.8.1 or later).
* CMake version 3.13 or later - Minimum CMake version for building the project.
* CppUnit 1.14.0 or later - To build unit tests.
* Doxygen 1.8.13 or later - To build documentation.

## Installation Instructions

This project is built with CMake, which prefers out-of-tree builds. The CMake variable `CMAKE_INSTALL_PREFIX` governs where the package is installed. Here is a sample build from the top-level directory of the cloned repository assuming a build directory called "build:"

```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/path/to/ddasformat
cmake --build build
cmake --install build
```
### Running DDASFormat Unit Tests

From the top-level directory, again assuming the build directory is called "build:"

```
ctest --test-dir build -VV
```

## Using DDASFormat in Other CMake Projects

DDASFormat is commonly incorporated into larger projects and can be built either as a Git submodule using `add_subdirectory()` or as an external project using CMake's `ExternalProject` module. The library is designed to detect when it is being built as a subproject and will disable its unit tests and documentation builds by default in that case.

### Using DDASFormat with `add_subdirectory()`

If DDASFormat is available in your source tree, add it to the build with:

```
add_subdirectory(DDASFormat)

target_link_libraries(my_application PRIVATE DDASFormat)
```

The DDASFormat library target will be built automatically as part of the parent project's build.

### Using DDASFormat with `ExternalProject_Add()`

Projects that prefer to build DDASFormat separately can use CMake's `ExternalProject` module:

```
include(ExternalProject)

ExternalProject_Add(
    DDASFormat
    GIT_REPOSITORY <repository-url>
    GIT_TAG <tag>
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ddasformat
)
```

This will configure, build, and install DDASFormat into the specified installation directory. The parent project can then add the appropriate include and library paths when building targets that depend on DDASFormat.

For example:

```
target_include_directories(my_application PRIVATE
    ${CMAKE_BINARY_DIR}/ddasformat/include
)

target_link_directories(my_application PRIVATE
    ${CMAKE_BINARY_DIR}/ddasformat/lib
)

target_link_libraries(my_application PRIVATE DDASFormat)
```

The exact library filename and installation paths may vary by platform.

### Using an Installed Copy of DDASFormat

DDASFormat can also be built and installed independently using the installation instructions above. Projects using an installed copy must manually specify include and library locations.

For example:

```
target_include_directories(my_application PRIVATE
    /path/to/ddasformat/include
)

target_link_directories(my_application PRIVATE
    /path/to/ddasformat/lib
)

target_link_libraries(my_application PRIVATE DDASFormat)
```

### Runtime Library Path (RPATH)

When using DDASFormat as a shared library, the runtime linker must be able to locate `libDDASFormat.so` at execution time.

This is handled by the consuming project and depends on its installation layout and build system. Common approaches in CMake include setting an RPATH, using `$ORIGIN`-based relative paths, or relying on system library paths.

No specific runtime configuration is enforced by DDASFormat.

## Using DDASFormat in a GNU Make Project

Projects that use traditional GNU Makefiles can link against an installed copy of DDASFormat by specifying the include directory, library directory, and runtime search path.

Example:

```
CXX      := g++
CXXFLAGS := -O2 -std=c++11 \
            -I/path/to/ddasformat/include

LDFLAGS  := -L/path/to/ddasformat/lib \
            -Wl,-rpath,/path/to/ddasformat/lib

LDLIBS   := -lDDASFormat

my_application: main.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
```

The `-Wl,-rpath,...` option embeds the DDASFormat library directory into the executable's runtime search path. This allows the dynamic loader to locate `libDDASFormat.so` without requiring users to set `LD_LIBRARY_PATH`.