# PowSyBl - IIDM

## Requirements

To build powsybl-iidm, you need:
- A C++ compiler that supports C++11 ([clang](https://clang.llvm.org) 3.3 or higher, [g++](https://gcc.gnu.org) 5.0 or higher)
- [CMake](https://cmake.org) (2.6 or higher)
- [Make](https://www.gnu.org/software/make/)
- [Boost](https://www.boost.org) development packages (1.56 or higher)
- [LibXML2](http://www.xmlsoft.org/) development packages

### Alpine
Run the following command to install the requirements:
```
$> apk add boost-dev clang cmake g++ make
```

### CentOS
Run the following command to install the requirements:
```
$> yum -y install boost-devel clang cmake gcc-c++ make
$> mkdir /opt/boost-1.69 && ln -s /usr/include/boost169 /opt/boost-1.69/include && ln -s /usr/lib64/boost169 /opt/boost-1.69/lib
```

### Ubuntu
Run the following command to install the requirements:
```
$> apt-get -y install clang cmake g++ libboost-all-dev make
```

### Docker

You can also use a pre-configured docker image for these environments. See the [readme](docker/README.md) to know how to
build these images.

