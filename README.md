# PowSyBl - IIDM for C++

[![Actions Status](https://github.com/powsybl/powsybl-iidm4cpp/workflows/CI/badge.svg)](https://github.com/powsybl/powsybl-iidm4cpp/actions)
[![Coverage Status](https://sonarcloud.io/api/project_badges/measure?project=com.powsybl%3Apowsybl-iidm4cpp&metric=coverage)](https://sonarcloud.io/component_measures?id=com.powsybl%3Apowsybl-iidm4cpp&metric=coverage)
[![Quality Gate](https://sonarcloud.io/api/project_badges/measure?project=com.powsybl%3Apowsybl-iidm4cpp&metric=alert_status)](https://sonarcloud.io/dashboard?id=com.powsybl%3Apowsybl-iidm4cpp)
[![MPL-2.0 License](https://img.shields.io/badge/license-MPL_2.0-blue.svg)](https://www.mozilla.org/en-US/MPL/2.0/)
[![Join the community on Spectrum](https://withspectrum.github.io/badge/badge.svg)](https://spectrum.chat/powsybl)

## Table of contents
- [Requirements](#requirements)
    - [Ubuntu 20.04](#ubuntu-2004)
    - [Ubuntu 18.04](#ubuntu-1804)
    - [CentOS 8](#centos-8)
    - [CentOS 7](#centos-7)
    - [Alpine](#alpine)
    - [Windows](#windows)
    - [MacOS](#macos)
- [Build from sources](#build-from-sources)
    - [Build the examples](#build-the-examples)
    - [Generate the documentation](#generate-the-documentation)
- [Link with powsybl-iidm4cpp](#link-with-powsybl-iidm4cpp)
- [Contributing](#contributing)

## Requirements

To build powsybl-iidm4cpp, you need:
- A C++ compiler that supports C++11 ([clang](https://clang.llvm.org) 3.3 or higher, [g++](https://gcc.gnu.org) 5.0 or higher)
- [CMake](https://cmake.org) (3.12 or higher)
- [Make](https://www.gnu.org/software/make/)
- [Boost](https://www.boost.org) development packages (1.56 or higher)
- [LibXML2](http://www.xmlsoft.org/) development packages

### Build from sources

### Ubuntu 20.04
```
$> apt install -y cmake g++ git libboost-all-dev libxml2-dev make
``` 

### Ubuntu 18.04
```
$> apt install -y g++ git libboost-all-dev libxml2-dev make wget
```

**Note:** Under Ubuntu 18.04, the default CMake package is too old (3.10), so you have to install it manually:
```
$> wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.tar.gz
$> tar xzf cmake-3.12.0-Linux-x86_64.tar.gz
$> export PATH=$PWD/cmake-3.12.0-Linux-x86_64/bin:$PATH
```

### CentOS 8
```
$> yum install -y boost-devel gcc-c++ git libxml2-devel make wget
```

**Note:** Under CentOS 8, the default CMake package is too old (3.11.4), so you have to install it manually:
```
$> wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.tar.gz
$> tar xzf cmake-3.12.0-Linux-x86_64.tar.gz
$> export PATH=$PWD/cmake-3.12.0-Linux-x86_64/bin:$PATH
```

### CentOS 7
```
$> yum install -y gcc-c++ git libxml2-devel make wget
```
**Note:** Under CentOS 7, the default `boost-devel` package is too old (1.53), so we install Boost 1.69 from `epel-release`.
```
$> yum install -y epel-release
$> yum install -y boost169-devel
$> export BOOST_INCLUDE_DIR=/usr/include/boost169
$> export BOOST_LIBRARY_DIR=/usr/lib64/boost169
```

**Note:** Under CentOS 7, the default CMake package is too old (2.8.12), so you have to install it manually:
```
$> wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.tar.gz
$> tar xzf cmake-3.12.0-Linux-x86_64.tar.gz
$> export PATH=$PWD/cmake-3.12.0-Linux-x86_64/bin:$PATH
```

### Alpine
```
$> apk add boost-dev cmake git g++ libxml2-dev make
```

### Windows
Under Windows, install all the following requirements following the vendor instructions:
- [Visual Studio](https://visualstudio.microsoft.com/fr/vs/) Community Edition
- [CMake](https://cmake.org/) (3.12 or higher)
- [Git](https://git-scm.com/download/win)

To build **Boost** (1.56 or higher), download the sources from [Boost website](https://www.boost.org/users/download) and uncompress them into a folder. Then open a MSVC prompt and run the following commands:
```
$> cd <BOOST_SOURCES>
$> b2 -j 4 --with-date_time --with-filesystem --with-program_options --with-system --with-test --layout=system --prefix=<BOOST_PREFIX> variant=<BOOST_BUILD_TYPE> architecture=x86 address-model=64 link=static,shared stage
$> b2 install
```
where `BOOST_PREFIX` is the folder where Boost libraries will be installed and the `BOOST_BUILD_TYPE` is the build type (`debug` or `release`).

To build **LibXml2**, download the sources from the [GitHub repository](https://github.com/GNOME/libxml2/releases) and uncompress them into a folder. Then open a MSVC prompt and run the following commands:
```
$> cd <LIBXML2_SOURCES>/win32
$> cscript configure.js compiler=msvc iconv=no prefix=<LIBXML2_PREFIX>
$> nmake /f Makefile.msvc
$> nmake /f Makefile.msvc install
```
where `LIBXML2_PREFIX` is the folder where LibXML2 libraries will be installed.

### MacOS
Under MacOS, install all the following requirements following the vendor instructions:
- XCode or Command Line Tools (with `xcode-select --install`)
- [CMake](https://cmake.org/) (3.12 or higher)

To install **Boost** using `brew`, run the following command:
```
$> brew install boost
```
Otherwise, you can build **Boost** (1.56 or higher) from the sources. First, download the sources from [Boost website](https://www.boost.org/users/download) and uncompress them into a folder. Then open a terminal and run the following commands:
```
$> cd <BOOST_SOURCES>
$> ./b2 -j 4 --with-date_time --with-filesystem --with-program_options --with-system --with-test --layout=system --prefix=<BOOST_PREFIX> variant=<BOOST_BUILD_TYPE> architecture=x86 address-model=64 link=static,shared stage
$> ./b2 install
```
where `BOOST_PREFIX` is the folder where Boost libraries will be installed and the `BOOST_BUILD_TYPE` is the build type (`debug` or `release`).

**Note:** Depending on the `BOOST_PREFIX`, we would need _root_ access to install the libraries.
```
$> sudo ./b2 install
```

## Build the sources

1 - Clone the project
```
$> git clone https://github.com/powsybl/powsybl-iidm4cpp
$> cd powsybl-iidm4cpp
```

2 - Configure the project
```
$> mkdir build
$> cd build
$> cmake .. -DCMAKE_INSTALL_PREFIX=<PREFIX> -DCMAKE_BUILD_TYPE=<BUILD_TYPE>
```
**Available options:**
- BUILD_DOXYGEN: Generate the API documentation using doxygen (Default is **OFF**)
- BUILD_EXAMPLES: Build the examples (Default is **OFF**)

**Note:** Under Windows the **Boost** libraries or **LibXML** libraries could be installed in a custom third parties folder, so you would have to help `cmake` to find the required packages, using the `-DCMAKE_PREFIX_PATH` options:
```
$> cmake .. -DCMAKE_INSTALL_PREFIX=<PREFIX> -DCMAKE_BUILD_TYPE=<BUILD_TYPE> -DCMAKE_PREFIX_PATH=<BOOST_PREFIX>;<LIBXML2_PREFIX>
``` 

**Note:** Under MacOS the **Boost** libraries could be installed in a custom third parties folder, so you would have to help `cmake` to find the required package, using the `-DCMAKE_PREFIX_PATH` options:
```
$> cmake .. -DCMAKE_INSTALL_PREFIX=<PREFIX> -DCMAKE_BUILD_TYPE=<BUILD_TYPE> -DCMAKE_PREFIX_PATH=<BOOST_PREFIX>
```

3. Build the sources
```
$> cmake --build .
```

4. Install the libraries
```
$> cmake --build . --target install
```

### Build the examples
To build the examples, you have to pass the `-DBUILD_EXAMPLES=ON` flag to the configure command, and build the sources:
```
$> cmake .. -DCMAKE_INSTALL_PREFIX=<PREFIX> -DCMAKE_BUILD_TYPE=<BUILD_TYPE> -DBUILD_EXAMPLES=ON
$> cmake --build .
``` 

Click [here](examples/README.md) to see the list of available examples. 

### Generate the documentation
This project uses [doxygen](http://www.doxygen.nl/) to generate code documentation. To generate the API documentation using doxygen, add `-DBUILD_DOXYGEN=ON` flag to the configure command line. This will create a new `doxygen` target.
```
$> cmake .. -DCMAKE_INSTALL_PREFIX=<PREFIX> -DCMAKE_BUILD_TYPE=<BUILD_TYPE> -DBUILD_DOXYGEN=ON
$> cmake --build . --target doxygen
```
The HTML documentation is available in `<BUILD_DIR>/doc/html/index.html`.

## Link with powsybl-iidm4cpp
We provide cmake script files that make it easy to use `powsybl-iidm4cpp` in a CMake project, that are installed in the `<PREFIX>/LibIIDM/cmake` folder.

To use the library in your project, add the `find_package` instruction to your `CMakeLists.txt` file:
```
find_package(LibIIDM REQUIRED)
``` 

Then configure your project passing the `-DCMAKE_PREFIX_PATH=<IIDM4CPP_PREFIX>` option to the cmake command:
```
$> cmake ... -DCMAKE_PREFIX_PATH=<IIDM4CPP_PREFIX>
```

A complete example is available [here](examples/example2). 

## Contributing

### Checkstyle
This project uses [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) to verify the code style. This tool is provided with the clang extra tools. To enable the code style checking, add the `-DCMAKE_CXX_CLANG_TIDY=clang-tidy` flag to the configure command.  

### Code coverage
This project uses either [gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) or [llvm-cov](https://llvm.org/docs/CommandGuide/llvm-cov.html) to compute the code coverage. We also use [gcovr](https://gcovr.com/en/stable/) (4.2 or higher) to generate both sonar and HTML reports. To compute the code coverage, add the `-DCODE_COVERAGE=TRUE` flag to the configure command.

**Note:** To have correct coverage results, you have to disable compiler and linker optimization, compiling the project in `Debug` mode.

```
$> cmake .. -DCMAKE_BUILD_TYPE=Debug -DCODE_COVERAGE=TRUE
$> cmake --build .
$> cmake --build . --target test
$> cmake --build . --target code-coverage
```
The HTML report is available in `<BUILD_DIR>/coverage/index.html`. 
