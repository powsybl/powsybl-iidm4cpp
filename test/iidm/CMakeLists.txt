#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    BatteryTest.cpp
    BusbarSectionTest.cpp
    BusBreakerVoltageLevelTest.cpp
    CountryTest.cpp
    CurrentLimitsTest.cpp
    DanglingLineTest.cpp
    EnergySourceTest.cpp
    GeneratorTest.cpp
    HvdcLineTest.cpp
    iidm.cpp
    LccConverterStationTest.cpp
    LineTest.cpp
    LoadTest.cpp
    NetworkFactory.cpp
    NetworkIndexTest.cpp
    NetworkTest.cpp
    NodeBreakerVoltageLevelTest.cpp
    PhaseTapChangerTest.cpp
    PropertiesTest.cpp
    RatioTapChangerTest.cpp
    ReactiveLimitsTest.cpp
    ShuntCompensatorTest.cpp
    StaticVarCompensatorTest.cpp
    SubstationTest.cpp
    ThreeWindingsTransformerTest.cpp
    TieLineTest.cpp
    TwoWindingsTransformerTest.cpp
    VoltageLevelTest.cpp
    VscConverterStationTest.cpp

    converter/ExportOptionsTest.cpp
    converter/FakeAnonymizerTest.cpp
    converter/ImportOptionsTest.cpp
)

add_executable(unit-tests-iidm ${UNIT_TEST_SOURCES})
target_include_directories(unit-tests-iidm
    PRIVATE Boost::boost
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
    PRIVATE ${CMAKE_HOME_DIRECTORY}/test
)
target_compile_definitions(unit-tests-iidm
    PRIVATE BOOST_TEST_DYN_LINK
 )
target_link_libraries(unit-tests-iidm
    iidm
)
