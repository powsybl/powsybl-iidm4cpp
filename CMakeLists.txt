#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(powsybl-iidm4cpp)

set(IIDM_VERSION_MAJOR 1)
set(IIDM_VERSION_MINOR 5)
set(IIDM_VERSION_PATCH 0)
set(IIDM_VERSION ${IIDM_VERSION_MAJOR}.${IIDM_VERSION_MINOR}.${IIDM_VERSION_PATCH})
set(IIDM_SOVERSION ${IIDM_VERSION_MAJOR})

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for headers")
set(INSTALL_CMAKE_DIR LibIIDM/cmake CACHE PATH "Installation directory for cmake files")
set(INSTALL_DOC_DIR share CACHE PATH "Installation directory for doxygen files")

option(BUILD_DOXYGEN "Enable/Disable the generation of the Doxygen." OFF)
option(BUILD_EXAMPLES "Enable/Disable the compilation of the examples." OFF)
option(BUILD_TESTS "Enable/Disable the compilation of unit tests." ON)
option(BUILD_TOOLS "Enable/Disable the compilation of tools." ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -Wswitch-enum")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
    # set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D_GLIBCXX_DEBUG")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4250 /wd4251 /wd4267 /wd4275")
endif ()

set(Boost_USE_STATIC_LIBS OFF)
if (BUILD_TESTS)
    find_package(Boost 1.65 REQUIRED COMPONENTS date_time filesystem program_options system unit_test_framework)
else ()
    find_package(Boost 1.65 REQUIRED COMPONENTS date_time filesystem program_options system)
endif()
find_package(LibXml2 REQUIRED)
find_package(Threads REQUIRED)

if (WIN32)
    # On windows, also force prefix to avoid conflict between import file (for shared compilation) and static file
    set(CMAKE_STATIC_LIBRARY_PREFIX lib)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

enable_testing()

set(CODE_COVERAGE FALSE CACHE BOOL "Enable code coverage")
if (CODE_COVERAGE)
    # if code-coverage is ON, force tests build
    set(BUILD_TESTS ON)

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
add_subdirectory(extensions)

if (BUILD_TOOLS)
    add_subdirectory(tools)
endif ()

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

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
