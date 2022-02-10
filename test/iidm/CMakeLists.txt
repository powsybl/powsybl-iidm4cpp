#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    ActivePowerLimitsTest.cpp
    AliasesTest.cpp
    ApparentPowerLimitsTest.cpp
    BatteryTest.cpp
    BranchTest.cpp
    BusbarSectionTest.cpp
    BusBreakerVoltageLevelTest.cpp
    BusTest.cpp
    ComponentsTest.cpp
    CountryTest.cpp
    CurrentLimitsTest.cpp
    DanglingLineTest.cpp
    EnergySourceTest.cpp
    ExtensionTest.cpp
    ExtensionAdderTest.cpp
    GeneratorTest.cpp
    HvdcLineTest.cpp
    iidm.cpp
    IdBasedBusRefTest.cpp
    IdentifiableTypeTest.cpp
    LccConverterStationTest.cpp
    LineTest.cpp
    LoadTest.cpp
    NetworkExtension.cpp
    NetworkFactory.cpp
    NetworkIndexTest.cpp
    NetworkTest.cpp
    NodeBreakerConnectTest.cpp
    NodeBreakerVoltageLevelTest.cpp
    PhaseTapChangerTest.cpp
    RatioTapChangerTest.cpp
    ReactiveLimitsTest.cpp
    ShuntCompensatorTest.cpp
    StaticVarCompensatorTest.cpp
    SubstationTest.cpp
    TerminalTest.cpp
    TopologyLevelTest.cpp
    ThreeWindingsTransformerTest.cpp
    TieLineTest.cpp
    TwoWindingsTransformerTest.cpp
    VariantManagerTest.cpp
    VoltageLevelTest.cpp
    VscConverterStationTest.cpp

    converter/ConversionParametersTest.cpp
    converter/ExportOptionsTest.cpp
    converter/FakeAnonymizerTest.cpp
    converter/ImportOptionsTest.cpp
    converter/SimpleAnonymizerTest.cpp

    extensions/LoadDetailTest.cpp
    extensions/SlackTerminalTest.cpp

    util/SVTest.cpp
    util/NodeBreakerTopologyTest.cpp
    util/TerminalFinderTest.cpp
)

add_executable(unit-tests-iidm ${UNIT_TEST_SOURCES})
target_compile_definitions(unit-tests-iidm PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(unit-tests-iidm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(unit-tests-iidm PRIVATE iidm-tests)

add_test(NAME iidm COMMAND unit-tests-iidm -- --resources=${CMAKE_SOURCE_DIR}/test/resources)

add_subdirectory(converter/xml)
