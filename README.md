# DDASFormat

## Introduction

Many NSCLDAQ and user applications using DDAS rely on methods which can transform formatted binary event data into something more useful, usually by parsing binary data into some object which provides an interface to access raw values (timestamp, energy, etc.). This process is colloquially referred to as "unpacking" the data and the codes which perform this task are therefore called "unpackers."

This project provides a single DDAS format library and unpacker which can be used in programs which process DDAS data e.g., the NSCLDAQ `ddasdumper` or SpecTcl. The format library can be used as-is on systems where NSCLDAQ is not installed as it does not depend on any other FRIB software packages.

The format library consists of the following:
* DAQ::DDAS::DDASHit - A class which encapsulates the information emitted by a Pixie-16 digitizer. It contains information for a single channel only.
* DAQ::DDAS::DDASHitUnpacker - Unpacks DDAS data recorded by NSCLDAQ into DDASHits.
* DDASBitMasks.h - A header file contianing masks used to extract information from specific parts of XIA Pixie list-mode data.

## Requirements

* `cmake` version 3.13.4+ - Minimum supported `cmake` version.
* gcc 4.8.1+ - Compiler support for the C++11 standard.
* CppUnit 1.14.0+ - To build unit tests.
* Doxygen 1.8.13+ - To build documentation.

## Installation instructions

This project is built with `cmake`, which prefers out-of-tree builds. There is only one `cmake` variable: `CMAKE_INSTALL_PREFIX`, which governs where the package is installed. Here is a sample build from the cloned repository:

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/opt/ddasformat/0.1
make all install test
```

Running the tests using `ctest -VV` will in general give better feedback than `make test` about which tests are failing and why. One may prefer to `make all install && ctest -VV`. For `cmake` 3.15+, one can use the `install` and `prefix` options of the `cmake` command:

```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install . --prefix "/usr/opt/ddasformat/1.1"
ctest -VV
```