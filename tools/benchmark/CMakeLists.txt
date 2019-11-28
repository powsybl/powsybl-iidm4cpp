#
# Copyright (c) 2019, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_BENCHMARK_SOURCES
    Benchmark.cpp
)

add_executable(iidm-benchmark ${IIDM_BENCHMARK_SOURCES})
target_include_directories(iidm-benchmark
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
    PRIVATE ${CMAKE_HOME_DIRECTORY}/src # to be able to access NetworkXml
)
target_compile_definitions(iidm-benchmark
    PRIVATE BOOST_TEST_DYN_LINK
)
target_link_libraries(iidm-benchmark
    iidm
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY}
    ${LIBXML2_LIBRARIES}
)
