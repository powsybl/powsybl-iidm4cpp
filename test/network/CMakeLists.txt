#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    EurostagFactoryTest.cpp
    network.cpp)

add_executable(unit-tests-network ${UNIT_TEST_SOURCES})
target_include_directories(unit-tests-network
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
    PRIVATE ${CMAKE_HOME_DIRECTORY}/test
)
target_compile_definitions(unit-tests-network
    PRIVATE BOOST_TEST_DYN_LINK
)
target_link_libraries(unit-tests-network
    iidm
    ${LIBXML2_LIBRARIES}
)
