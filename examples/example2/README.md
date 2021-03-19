# Example 2 - Use iidm4cpp in a CMake project

This example shows how to create a cmake project that uses iidm4cpp. This example uses the source code of the [example 1](../example1) to produce the same executable, but as an external CMake project.

## Requirements
To build this example, you need to compile and install the libraries following the [build instructions](../../README.md#build-from-sources). In this example, we'll consider that the libraries have been install in the `/tmp/powsybl-iidm4cpp` folder:
```
$> cd powsybl-iidm4cpp
$> mkdir build
$> cd build
$> cmake .. -DCMAKE_INSTALL_PREFIX=/tmp/powsybl-iidm4cpp
$> make
$> make install
```

During the installation, the headers, the libraries and the cmake scripts are copied to the prefix directory:
```
$> tree /tmp/powsybl-iidm4cpp
/tmp/powsybl-iidm4cpp
├── include
│   └── powsybl
│       ├── AssertionError.hpp
│       ├── iidm/...
│       ├── logging/...
│       ├── math/...
│       ├── network/...
│       ├── PowsyblException.hpp
│       ├── stdcxx/...
│       ├── test/...
│       └── xml/...
├── lib
│   ├── libiidm.a
│   ├── libiidm-ext-entsoe.a
│   ├── libiidm-ext-entsoe.so -> libiidm-ext-entsoe.so.1
│   ├── libiidm-ext-entsoe.so.1 -> libiidm-ext-entsoe.so.1.1.0
│   ├── libiidm-ext-entsoe.so.1.1.0
│   ├── libiidm-ext-iidm.a
│   ├── libiidm-ext-iidm.so -> libiidm-ext-iidm.so.1
│   ├── libiidm-ext-iidm.so.1 -> libiidm-ext-iidm.so.1.1.0
│   ├── libiidm-ext-iidm.so.1.1.0
│   ├── libiidm.so -> libiidm.so.1
│   ├── libiidm.so.1 -> libiidm.so.1.1.0
│   ├── libiidm.so.1.1.0
│   ├── libiidm-tests.so
│   └── libiidm-tests-static.a
└── LibIIDM
    └── cmake
        ├── libiidm-config.cmake
        ├── libiidm-config-debug.cmake
        └── libiidm-config-version.cmake
``` 

## Create the cmake project
The cmake scripts, installed in the `<PREFIX>/LibIIDM/cmake` are used by the `find_package` instruction to load all the exported targets during the configuration of the project. The `CMakeLists.txt` below shows how to create a small project using iidm4cpp:
```
# Force a minimum CMake version
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Declare the project
project(my-project)

# Find the required dependencies: Boost, LibXML2 and IIDM and import the targets
find_package(Boost 1.65 REQUIRED COMPONENTS date_time filesystem program_options system unit_test_framework)
find_package(LibXml2 REQUIRED)
find_package(LibIIDM REQUIRED)

# Create an executable
add_executable(my-executable ../example1/main.cpp)

# Link the executable with the IIDM shared library
target_link_libraries(my-executable PRIVATE IIDM::iidm)
```

## Configure and build the project
To configure the project, pass the `-DCMAKE_PREFIX_PATH=<IIDM4CPP_PREFIX>` option to the cmake command, where `IIDM4CPP_PREFIX` is the folder where the libraries have been installed. In this example, the libraries have been installed in the `/tmp/powsybl-iidm4cpp` directory:
```
$> cmake -DCMAKE_PREFIX_PATH=/tmp/powsybl-iidm4cpp
-- Boost version: 1.66.0
-- Found the following Boost libraries:
--   date_time
--   filesystem
--   program_options
--   system
--   unit_test_framework
-- Configuring done
-- Generating done
-- Build files have been written to: /home/user/powsybl-iidm4cpp/examples/example2/build
```

Once the project have been configured, compile the project running the following command:
```
$> cmake --build .
[ 50%] Building CXX object CMakeFiles/my-executable.dir/home/user/powsybl-iidm4cpp/examples/example1/main.cpp.o
[100%] Linking CXX executable my-executable
[100%] Built target my-executable
```

## Run the program
You can run the program we just compile, running the following command:
```
$> ./my-executable
Usage: ./my-executable <input-file> <output-file>
```

## Going further
In this example, the executable `my-executable` is linked to the shared library:
```
$> ldd my-executable 
        linux-vdso.so.1 (0x00007ffc6f51c000)
        libiidm.so.1 => /tmp/powsybl-iidm4cpp/lib/libiidm.so.1 (0x00007f8c38f03000)
        libxml2.so.2 => /lib64/libxml2.so.2 (0x00007f8c38d70000)
        libboost_filesystem.so.1.66.0 => /lib64/libboost_filesystem.so.1.66.0 (0x00007f8c38d53000)
        libboost_system.so.1.66.0 => /lib64/libboost_system.so.1.66.0 (0x00007f8c38d4c000)
        libdl.so.2 => /lib64/libdl.so.2 (0x00007f8c38d46000)
        libstdc++.so.6 => /lib64/libstdc++.so.6 (0x00007f8c38bae000)
        libm.so.6 => /lib64/libm.so.6 (0x00007f8c38a28000)
        libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00007f8c38a0d000)
        libc.so.6 => /lib64/libc.so.6 (0x00007f8c38847000)
        libz.so.1 => /lib64/libz.so.1 (0x00007f8c3882d000)
        liblzma.so.5 => /lib64/liblzma.so.5 (0x00007f8c38804000)
        librt.so.1 => /lib64/librt.so.1 (0x00007f8c387fa000)
        libpthread.so.0 => /lib64/libpthread.so.0 (0x00007f8c387d7000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f8c3ad23000)
```

It's also possible to link with the static version, by replacing the `IIDM::iidm` target by `IIDM::iidm-static` in the `CMakeLists.txt` file:
```
# Link the executable with the IIDM static library
target_link_libraries(my-executable PRIVATE IIDM::iidm-static)
```

Then, rebuild the project:
```
$> cmake --build .
-- Boost version: 1.66.0
-- Found the following Boost libraries:
--   date_time
--   filesystem
--   program_options
--   system
--   unit_test_framework
-- Configuring done
-- Generating done
-- Build files have been written to: /home/user/powsybl-iidm4cpp/examples/example2/build
Scanning dependencies of target my-executable
[ 50%] Building CXX object CMakeFiles/my-executable.dir/home/user/powsybl-iidm4cpp/examples/example1/main.cpp.o
[100%] Linking CXX executable my-executable
[100%] Built target my-executable
```
