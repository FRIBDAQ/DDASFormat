# DDASFormat

## Introduction

Many NSCLDAQ and user applications using DDAS rely on methods which can transform binary event data into a more useful format, usually by parsing the binary data into some object which provides an interface to access the raw data values (timestamp, energy, etc.). This process is colloquially referred to as "unpacking" the data and the codes which perform this task are therefore called "unpackers."

This project aims to provide a DDAS format library which can be used in programs which make use of DDAS data unpackers e.g., the NSCLDAQ `ddasdumper` or SpecTcl. The format library can be used as as-is on systems where NSCLDAQ is not installed.

The format library consists of:
* DDASHit - The DDASHit class encapsulates the information that is emitted by the Pixie-16 digitizer for a single event. It contains information for a single channel only.
* DDASHitUnpacker - Unpacks DDAS data recorded by NSCLDAQ into DDASHits.
* DDASBitMasks - Masks to extract information from specific parts of XIA Pixie list-mode data.

## Requirements

* `cmake` version 3.13+ - Minimum supported `cmake` version.
* gcc 4.8.1+ - Compiler support for c++11 standard.
* CppUnit - To build unit tests.

## Installation instructions

This project is built with `cmake`, which prefers out-of-tree builds. There is only one `cmake` variable: `CMAKE_INSTALL_PREFIX`, which governs where the package is installed. Here is a sample build from the cloned repository:

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/opt/ddasformat/0.1
make all install
```

For `cmake` 3.15+, one can use the `install` and `prefix` options of the `cmake` command:

```
mkdir build
cd build
cmake ..
cmake --build .
cmake --install . --prefix "/usr/opt/ddasformat/0.1"
```