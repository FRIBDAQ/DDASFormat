# DDASFormat

## Introduction

Many NSCLDAQ and user applications using DDAS rely on methods which can transform formatted binary event data into something more useful, usually by parsing binary data into some object which provides an interface to access raw values (timestamp, energy, etc.). This process is colloquially referred to as "unpacking" the data and the codes which perform this task are therefore called "unpackers."

This project provides a single DDAS format library and unpacker which can be used in programs which process DDAS data e.g., the NSCLDAQ ddasdumper or SpecTcl. The format library can be used as-is on systems where NSCLDAQ is not installed as it does not depend on any other FRIB software packages.

The format library consists of the following:
* ddasfmt::DDASHit - A class which encapsulates the information emitted by a Pixie-16 digitizer. It contains information for a single channel only.
* ddasfmt::DDASHitUnpacker - Unpacks DDAS data recorded by NSCLDAQ into DDASHits.
* DDASBitMasks.h - A header file containing masks used to extract information from specific parts of XIA Pixie list-mode data. Bitmasks are also in the ddasfmt namespace.

## Requirements

The minimum software requirements are listed below. The code has been built and tested extensively on Debian 10 (buster) and Debian 11 (bullseye) systems at FRIB.

* CMake version 3.13.4 - Required CMake version for building the project.
* A C++ compiler with support for the C++11 standard (e.g., GCC 4.8.1 or later)
* CppUnit 1.14.0 - To build unit tests.
* Doxygen 1.8.13 - To build documentation.

## Installation instructions

This project is built with CMake, which prefers out-of-tree builds. The CMake variable `CMAKE_INSTALL_PREFIX` governs where the package is installed. Here is a sample build from the top-level directory of the cloned repository:

```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/path/to/your/install/dir
cmake --build build
cmake --install build
```

Running the tests using `ctest -VV` will in general give better feedback than `make test` about which tests are failing and why. From the top-level directory:

```
ctest --test-dir build/ -VV
```

## Using DDASFormat in Other CMake Projects

DDASFormat is commonly incorporated into larger projects and can be built either as a Git submodule using `add_subdirectory()` or as an external project using CMake's `ExternalProject` module. The library is designed to detect when it is being built as a subproject and will disable its unit tests and documentation builds by default in that case.

### Using DDASFormat with `add_subdirectory()`

If DDASFormat is available in your source tree, add it to the build with:

```
cmake
add_subdirectory(DDASFormat)

target_link_libraries(my_application PRIVATE DDASFormat)
```

The DDASFormat library target will be built automatically as part of the parent project's build.

### Using DDASFormat with `ExternalProject_Add()`

Projects that prefer to build DDASFormat separately can use CMake's `ExternalProject` module:

```
cmake
include(ExternalProject)

ExternalProject_Add(
    DDASFormat
    GIT_REPOSITORY <repository-url>
    GIT_TAG main
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ddasformat
)
```

This will configure, build, and install DDASFormat into the specified installation directory. The parent project can then add the appropriate include and library paths when building targets that depend on DDASFormat.

For example:

```
cmake
target_include_directories(my_application PRIVATE
    ${CMAKE_BINARY_DIR}/ddasformat/include
)

target_link_libraries(my_application PRIVATE
    ${CMAKE_BINARY_DIR}/ddasformat/lib/libDDASFormat.so
)
```

The exact library filename and installation paths may vary by platform.

### Using an Installed Copy of DDASFormat

DDASFormat can also be built and installed independently using the installation instructions above. Projects using an installed copy must manually specify include and library locations.

For example:

```
cmake
target_include_directories(my_application PRIVATE
    /path/to/install/include
)

target_link_libraries(my_application PRIVATE
    /path/to/install/lib/libDDASFormat.so
)
```

The exact library filename may vary by platform.
