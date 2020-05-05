#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(powsybl-iidm4cpp)

set(IIDM_VERSION_MAJOR 1)
set(IIDM_VERSION_MINOR 1)
set(IIDM_VERSION_PATCH 0)
set(IIDM_VERSION ${IIDM_VERSION_MAJOR}.${IIDM_VERSION_MINOR}.${IIDM_VERSION_PATCH})
set(IIDM_SOVERSION ${IIDM_VERSION_MAJOR})

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for headers")
set(INSTALL_CMAKE_DIR LibIIDM/cmake CACHE PATH "Installation directory for cmake files")
set(INSTALL_DOC_DIR share CACHE PATH "Installation directory for doxygen files")

option(BUILD_DOXYGEN "Enable/Disable the generation of the Doxygen." OFF)

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
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4250 /wd4251 /wd4267 /wd4275")
endif ()

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.56 REQUIRED COMPONENTS date_time filesystem program_options system unit_test_framework)
find_package(LibXml2 REQUIRED)

if (WIN32)
    # On windows, also force prefix to avoid conflict between import file (for shared compilation) and static file
    set(CMAKE_STATIC_LIBRARY_PREFIX lib)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

enable_testing()

# Since Boost 1.60, program arguments and boost test arguments are separated by --
# https://github.com/boostorg/test/commit/d63379a9b3ad33d263d55c119681047c1d28547c
if ((DEFINED Boost_VERSION_MACRO AND ${Boost_VERSION_MACRO} GREATER 106000) OR (${Boost_VERSION} GREATER 106000))
    set(BOOST_ARGS_SEPARATOR "--")
endif ()

set(CODE_COVERAGE FALSE CACHE BOOL "Enable code coverage")
if (CODE_COVERAGE)
    include(CodeCoverage)
    code_coverage(NAME code-coverage
        OUTPUT_DIR coverage
        EXCLUDE_DIRS test tools)
endif ()

if (BUILD_DOXYGEN)
    include(Doxygen)
    doxygen(NAME doxygen CONFIG doxygen/Doxyfile.in OUTPUT_DIR doc)
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
    "${CMAKE_CURRENT_BINARY_DIR}/libiidm-config-version.cmake"
    VERSION ${IIDM_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libiidm-config-version.cmake"
    DESTINATION ${INSTALL_CMAKE_DIR}
)