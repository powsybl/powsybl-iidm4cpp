#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    AcDcConverterTest.cpp
    ActivePowerLimitsTest.cpp
    AliasesTest.cpp
    ApparentPowerLimitsTest.cpp
    AreaTest.cpp
    BatteryTest.cpp
    BranchTest.cpp
    BusbarSectionTest.cpp
    BusBreakerVoltageLevelTest.cpp
    BusTest.cpp
    ComponentsTest.cpp
    ConnectableTest.cpp
    ConvertTopologyTest.cpp
    CountryTest.cpp
    CurrentLimitsTest.cpp
    DanglingLineTest.cpp
    DcGroundTest.cpp
    DcLineTest.cpp
    DcNodeTest.cpp
    DcSwitchTest.cpp
    DcTerminalTest.cpp
    DroopCurveTest.cpp
    EnergySourceTest.cpp
    ExtensionTest.cpp
    ExtensionAdderTest.cpp
    ForkConnectDisconnectTest.cpp
    GeneratorTest.cpp
    GroundTest.cpp
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
    OperationalLimitsGroupsTest.cpp
    OperationalLimitsGroupTest.cpp
    OverloadManagementSystemTest.cpp
    PhaseTapChangerTest.cpp
    RatioTapChangerTest.cpp
    ReactiveLimitsTest.cpp
    SidesTest.cpp
    ShuntCompensatorTest.cpp
    StaticVarCompensatorTest.cpp
    SubnetworkCreationTest.cpp
    SubnetworkExplorationTest.cpp
    SubstationTest.cpp
    SwitchPredicateTest.cpp
    TerminalNumberTest.cpp
    TerminalTest.cpp
    TopologyLevelTest.cpp
    TopologyTraverseDepthAndBreadthTest.cpp
    TopologyTraverserTest.cpp
    ThreeWindingsTransformerTest.cpp
    TieLineTest.cpp
    TwoWindingsTransformerTest.cpp
    VariantManagerTest.cpp
    VoltageAngleLimitTest.cpp
    VoltageLevelTest.cpp
    VscConverterStationTest.cpp

    converter/AbstractOptionsTest.cpp
    converter/ConversionParametersTest.cpp
    converter/ExportOptionsTest.cpp
    converter/FakeAnonymizerTest.cpp
    converter/ImportOptionsTest.cpp
    converter/ParameterTest.cpp
    converter/SimpleAnonymizerTest.cpp

    extensions/LoadDetailTest.cpp
    extensions/SlackTerminalTest.cpp

    util/NetworksTest.cpp
    util/NodeBreakerTopologyTest.cpp
    util/SVTest.cpp
    util/TerminalFinderTest.cpp
    util/TieLineUtilTest.cpp
)

add_executable(unit-tests-iidm ${UNIT_TEST_SOURCES})
target_compile_definitions(unit-tests-iidm PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(unit-tests-iidm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(unit-tests-iidm PRIVATE iidm-tests)

add_test(NAME iidm COMMAND unit-tests-iidm -- --resources=${CMAKE_SOURCE_DIR}/test/resources)

add_subdirectory(converter/xml)
