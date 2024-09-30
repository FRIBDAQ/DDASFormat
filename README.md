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
* gcc 4.8.1 - Compiler support for the C++11 standard.
* CppUnit 1.14.0 - To build unit tests.
* Doxygen 1.8.13 - To build documentation.

## Installation instructions

This project is built with CMake, which prefers out-of-tree builds. The CMake variable `CMAKE_INSTALL_PREFIX` governs where the package is installed. Here is a sample build from the cloned repository:

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/opt/ddasformat
make all install test
```

Running the tests using `ctest -VV` will in general give better feedback than `make test` about which tests are failing and why. One may prefer to `make all install && ctest -VV`. For CMake 3.15+, one can use the `install` and `prefix` options of the `cmake` command:

```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install . --prefix "/usr/opt/ddasformat"
ctest -VV
```