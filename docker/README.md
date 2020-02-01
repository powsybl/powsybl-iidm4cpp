# Powsybl - IIDM for C++ - Docker

We provides several docker images to build powsybl-iidm. Supported environments are:
- [Alpine](https://alpinelinux.org) 3.9
- [CentOS](https://www.centos.org) 7.6
- [Ubuntu](https://ubuntu.com) 18.04

# Alpine

The Alpine image provides the following tools:
- CMake 3.13.0
- G++ 8.3.0
- Clang 5.0.2
- Boost 1.67
- LibXml2 2.9.9

To build the Alpine image, run the following command:
```
$> docker build -t powsybl-iidm4cpp:alpine -f docker/Dockerfile.alpine .
```

# CentOS

The CentOS image provides the following tools:
- CMake 2.8.12
- G++ 4.8.5
- Clang 3.4.2
- Boost 1.69
- LibXml2 2.9.1

To build the CentOS image, run the following command:
```
$> docker build -t powsybl-iidm4cpp:centos -f docker/Dockerfile.centos .
```

# Ubuntu

The Ubuntu image provides the following tools:
- CMake 3.10.2
- G++ 7.4.0
- Clang, Clang-tidy 6.0.0
- Boost 1.65.1
- LibXml2 2.9.4

To build the Ubuntu image, run the following command:
```
$> docker build -t powsybl-iidm4cpp:ubuntu -f docker/Dockerfile.ubuntu .
```
