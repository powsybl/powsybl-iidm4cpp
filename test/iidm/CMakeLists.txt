#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    BusbarSectionTest.cpp
    BusBreakerVoltageLevelTest.cpp
    CountryTest.cpp
    DanglingLineTest.cpp
    EnergySourceTest.cpp
    GeneratorTest.cpp
    LccConverterStationTest.cpp
    LineTest.cpp
    LoadTest.cpp
    NetworkFactory.cpp
    NetworkTest.cpp
    NodeBreakerVoltageLevelTest.cpp
    ObjectStoreTest.cpp
    ReactiveLimitsTest.cpp
    ShuntCompensatorTest.cpp
    StaticVarCompensatorTest.cpp
    SubstationTest.cpp
    TieLineTest.cpp
    VoltageLevelTest.cpp
    VscConverterStationTest.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(unit-tests-iidm ${UNIT_TEST_SOURCES})
target_link_libraries(unit-tests-iidm pthread ${GTEST_BOTH_LIBRARIES} iidm)
