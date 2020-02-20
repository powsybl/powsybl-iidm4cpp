# PowSyBl - IIDM for C++

[![Actions Status](https://github.com/powsybl/powsybl-iidm4cpp/workflows/CI/badge.svg)](https://github.com/powsybl/powsybl-iidm4cpp/actions)
[![Coverage Status](https://sonarcloud.io/api/project_badges/measure?project=com.powsybl%3Apowsybl-iidm4cpp&metric=coverage)](https://sonarcloud.io/component_measures?id=com.powsybl%3Apowsybl-iidm4cpp&metric=coverage)
[![Quality Gate](https://sonarcloud.io/api/project_badges/measure?project=com.powsybl%3Apowsybl-iidm4cpp&metric=alert_status)](https://sonarcloud.io/dashboard?id=com.powsybl%3Apowsybl-iidm4cpp)
[![MPL-2.0 License](https://img.shields.io/badge/license-MPL_2.0-blue.svg)](https://www.mozilla.org/en-US/MPL/2.0/)
[![Join the community on Spectrum](https://withspectrum.github.io/badge/badge.svg)](https://spectrum.chat/powsybl)

## Requirements

To build powsybl-iidm4cpp, you need:
- A C++ compiler that supports C++11 ([clang](https://clang.llvm.org) 3.3 or higher, [g++](https://gcc.gnu.org) 5.0 or higher)
- [CMake](https://cmake.org) (3.12 or higher)
- [Make](https://www.gnu.org/software/make/)
- [Boost](https://www.boost.org) development packages (1.56 or higher)
- [LibXML2](http://www.xmlsoft.org/) development packages

To install these requirements, the simplest way is to use the package manager:

### Alpine
```
$> apk add boost-dev clang cmake g++ make
```

### CentOS
```
$> yum -y install boost-devel clang cmake gcc-c++ make
```

### Ubuntu
```
$> apt-get -y install clang cmake g++ libboost-all-dev make
```

**Note**: On Ubuntu 18.04, default cmake package is too old (3.10), so you have to install it manually:
```
$> wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.tar.gz
$> tar xzf cmake-3.12.0-Linux-x86_64.tar.gz
$> export PATH=$PWD/cmake-3.12.0-Linux-x86_64/bin:$PATH
```

## Build
To build powsybl-iidm4cpp, run the following commands:
```
$> git clone https://github.com/powsybl/powsybl-iidm4cpp
$> cd powsybl-iidm4cpp
$> mkdir build
$> cd build
$> cmake .. -DCMAKE_INSTALL_PREFIX=<PREFIX> -DCMAKE_BUILD_TYPE=<BUILD_TYPE>
$> make -j
$> make install
```
