# PowSyBl - IIDM

## Requirements

To build powsybl-iidm, you need:
- A C++ compiler that supports C++11 ([clang](https://clang.llvm.org) 3.3 or higher, [g++](https://gcc.gnu.org) 5.0 or higher)
- [CMake](https://cmake.org) (2.6 or higher)
- [Make](https://www.gnu.org/software/make/)
- [Boost](https://www.boost.org) development packages
- [Google Test](https://github.com/google/googletest) development packages

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

You can also create a docker image based on [Ubuntu](https://www.ubuntu.com), ready to build powsybl-iidm:
```
$> docker build -t powsybl-iidm:centos -f docker/Dockerfile.centos .
```

### CentOS
Run the following command to install the requirements:
```
$> yum -y install boost-devel clang cmake gcc-c++ gtest-devel make
```

You can also create a docker image based on [CentOS](https://www.centos.org), ready to build powsybl-iidm:
```
$> docker build -t powsybl-iidm:centos -f docker/Dockerfile.centos .
```

### Alpine
Run the following command to install the requirements:
```
$> apk add boost-dev clang cmake g++ gtest gtest-dev make
```

You can also create a docker image based on [Alpine Linux](https://alpinelinux.org), ready to build powsybl-iidm:
```
$> docker build -t powsybl-iidm:alpine -f docker/Dockerfile.alpine .
```
