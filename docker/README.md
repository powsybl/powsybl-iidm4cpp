# Powsybl - IIDM - Docker

We provides several docker images to build powsybl-iidm. Supported environments are:
- [Alpine](https://alpinelinux.org) 3.9
- [CentOS](https://www.centos.org) 7.6
- [Ubuntu](https://alpinelinux.org) 18.04

# Alpine

The Alpine image provides the following tools:
- CMake 3.13.0
- G++ 8.3.0
- CLang 5.0.2
- Boost 1.67
- Google Test 1.8.1

To build the Alpine image, run the following command:
```
$> docker build -t powsybl-iidm:alpine -f docker/Dockerfile.alpine .
```

# CentOS

The CentOS image provides the following tools:
- CMake 2.8.12
- G++ 4.8.5
- CLang 3.4.2
- Boost 1.53
- Google Test 1.6.0

To build the CentOS image, run the following command:
```
$> docker build -t powsybl-iidm:centos -f docker/Dockerfile.centos .
```

# Ubuntu

The Ubuntu image provides the following tools:
- CMake 3.10.2
- G++ 5.5.0 / 6.5.0 / 7.4.0 / 8.3.0
- CLang 3.9.1 / 4.0.1 / 6.0.0 / 7.0.0
- Boost 1.65.1
- Google Test 1.8.0

To build the Ubuntu image, run the following command:
```
$> docker build -t powsybl-iidm:ubuntu -f docker/Dockerfile.ubuntu .
```
