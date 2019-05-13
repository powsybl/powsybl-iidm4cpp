# PowSyBl - IIDM

## Requirements

To build powsybl-iidm, you need:
- A C++ compiler that supports C++11 ([clang](https://clang.llvm.org) 3.3 or higher, [g++](https://gcc.gnu.org) 5.0 or higher)
- [CMake](https://cmake.org) (2.6 or higher)
- [Make](https://www.gnu.org/software/make/)
- [Boost](https://www.boost.org) development packages
- [Google Test](https://github.com/google/googletest) development packages

### Alpine
Run the following command to install the requirements:
```
$> apk add boost-dev clang cmake g++ gtest gtest-dev make
```

### CentOS
Run the following command to install the requirements:
```
$> yum -y install boost-devel clang cmake gcc-c++ gtest-devel make
```

### Ubuntu
Run the following command to install the requirements:
```
$> apt-get -y install clang cmake g++ libgtest-dev libboost-dev make
```

To build the unit tests, you need to compile Google Test.
```
$> cd /usr/include/gtest
$> cmake .
$> make install
```

### Docker

You can also use a pre-configured docker image for these environments. See the [readme](docker/README.md) to know how to
build these images.

