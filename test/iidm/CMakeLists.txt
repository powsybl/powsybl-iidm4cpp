#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    BatteryTest.cpp
    BranchTest.cpp
    BusbarSectionTest.cpp
    BusBreakerVoltageLevelTest.cpp
    BusTest.cpp
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
    TopologyLevelTest.cpp
    ThreeWindingsTransformerTest.cpp
    TieLineTest.cpp
    TwoWindingsTransformerTest.cpp
    VoltageLevelTest.cpp
    VscConverterStationTest.cpp

    converter/ExportOptionsTest.cpp
    converter/FakeAnonymizerTest.cpp
    converter/ImportOptionsTest.cpp
    converter/RoundTrip.cpp

    converter/xml/BatteryRoundTripTest.cpp
    converter/xml/FictitiousSwitchTest.cpp
    converter/xml/InternalConnectionsRoundTripTest.cpp
    converter/xml/LinesRoundTripTest.cpp
    converter/xml/LccRoundTripRef.cpp
    converter/xml/NetworkXmlTest.cpp
    converter/xml/NetworkXmlReaderContextTest.cpp
    converter/xml/NetworkXmlWriterContextTest.cpp
    converter/xml/RegulatingTerminalRoundTripTest.cpp
    converter/xml/StaticVarCompensatorRoundTripTest.cpp
    converter/xml/TerminalRefTest.cpp
    converter/xml/ThreeWindingsTransformerRoundTripTest.cpp
    converter/xml/XmlRoundTripTest.cpp
    converter/xml/VscRoundTripTest.cpp

    extensions/cvg/ActiveSeasonTest.cpp
    extensions/cvg/BusbarSectionPositionTest.cpp
    extensions/cvg/CongestionManagementTest.cpp
    extensions/cvg/ConnectablePositionTest.cpp
    extensions/cvg/GeneratorEntsoeCategoryTest.cpp
    extensions/cvg/StateOfChargeTest.cpp
    extensions/cvg/VoltageRegulationTest.cpp

    extensions/entsoe/EntsoeAreaTest.cpp
    extensions/entsoe/MergedXnodeTest.cpp
    extensions/entsoe/XnodeTest.cpp

    extensions/iidm/ActivePowerControlTest.cpp
    extensions/iidm/CoordinatedReactiveControlTest.cpp

    extensions/shortcircuits/BatteryShortCircuitsTest.cpp
    extensions/shortcircuits/GeneratorShortCircuitsTest.cpp
    extensions/shortcircuits/VoltageLevelShortCircuitsTest.cpp
)

add_executable(unit-tests-iidm ${UNIT_TEST_SOURCES})
target_include_directories(unit-tests-iidm
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
    PRIVATE ${CMAKE_HOME_DIRECTORY}/test
)
target_compile_definitions(unit-tests-iidm
    PRIVATE BOOST_TEST_DYN_LINK
)
target_link_libraries(unit-tests-iidm
    iidm
    ${LIBXML2_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SYSTEM_LIBRARY}
)
