#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(powsybl-iidm4cpp)

set(IIDM_VERSION_MAJOR 0)
set(IIDM_VERSION_MINOR 9)
set(IIDM_VERSION_PATCH 0)
set(IIDM_VERSION ${IIDM_VERSION_MAJOR}.${IIDM_VERSION_MINOR}.${IIDM_VERSION_PATCH})
set(IIDM_SOVERSION ${IIDM_VERSION_MAJOR})

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for headers")
set(INSTALL_CMAKE_DIR share/cmake CACHE PATH "Installation directory for cmake files")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (${CMAKE_VERSION} VERSION_LESS "3.1.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
else ()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -Wswitch-enum")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
    # set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D_GLIBCXX_DEBUG")
endif ()

find_package(Boost 1.56 REQUIRED COMPONENTS date_time filesystem program_options system unit_test_framework)
find_package(LibXml2 REQUIRED)

enable_testing()

# Since Boost 1.60, program arguments and boost test arguments are separated by --
# https://github.com/boostorg/test/commit/d63379a9b3ad33d263d55c119681047c1d28547c
if (${Boost_VERSION} GREATER 106000)
    set(BOOST_ARGS_SEPARATOR "--")
endif()

set(CODE_COVERAGE FALSE CACHE BOOL "Enable code coverage")
if (CODE_COVERAGE)
    include(CodeCoverage)
    code_coverage(NAME code-coverage
        OUTPUT_DIR coverage
        EXCLUDE_DIRS test tools)
endif ()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(extensions)
add_subdirectory(tools)

install(EXPORT iidm-targets
    NAMESPACE IIDM::
    FILE libiidm-config.cmake
    DESTINATION ${INSTALL_CMAKE_DIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libiidm-config-version.cmake
    VERSION ${IIDM_VERSION}
    COMPATIBILITY AnyNewerVersion
)
