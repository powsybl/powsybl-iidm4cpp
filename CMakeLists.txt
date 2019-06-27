#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(powsybl-iidm)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (${CMAKE_VERSION} VERSION_LESS "3.1.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
else ()
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D_GLIBCXX_DEBUG")
endif ()

find_package(Boost REQUIRED COMPONENTS date_time unit_test_framework)

set(CODE_COVERAGE FALSE CACHE BOOL "Enable code coverage")
if (CODE_COVERAGE)
    include(CodeCoverage)
    code_coverage(NAME code-coverage
        OUTPUT_DIR coverage
        EXCLUDE_DIRS test
        DEPENDENCIES tests)
endif ()

add_subdirectory(src)
add_subdirectory(test)
