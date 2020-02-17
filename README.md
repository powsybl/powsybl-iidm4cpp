# PowSyBl - IIDM for C++

## Requirements

To build powsybl-iidm4cpp, you need:
- A C++ compiler that supports C++11 ([clang](https://clang.llvm.org) 3.3 or higher, [g++](https://gcc.gnu.org) 5.0 or higher)
- [CMake](https://cmake.org) (2.6 or higher)
- [Make](https://www.gnu.org/software/make/)
- [Boost](https://www.boost.org) development packages (1.56 or higher)
- [LibXML2](http://www.xmlsoft.org/) development packages

To install these requirements, the simplest way is to use the package manager:

**Alpine**
```
$> apk add boost-dev clang cmake g++ make
```

**CentOS**
```
$> yum -y install boost-devel clang cmake gcc-c++ make
```

**Ubuntu**
```
$> apt-get -y install clang cmake g++ libboost-all-dev make
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
