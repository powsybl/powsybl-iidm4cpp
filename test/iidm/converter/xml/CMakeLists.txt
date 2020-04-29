#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(EXT_NAME ext-mock)

set(EXT_SOURCES
    extensions/ExtensionMock.cpp
    extensions/LoadMockExt.cpp
    extensions/LoadMockExtXmlSerializer.cpp
    extensions/LoadQuxExt.cpp
    extensions/LoadQuxXmlSerializer.cpp
    extensions/TerminalMockExt.cpp
    extensions/TerminalMockXmlSerializer.cpp
)

set(UNIT_TEST_SOURCES
    BatteryRoundTripTest.cpp
    EurostagTest.cpp
    FictitiousSwitchTest.cpp
    IdentifiableExtensionXmlSerializerTest.cpp
    InternalConnectionsRoundTripTest.cpp
    LinesRoundTripTest.cpp
    LccRoundTripRef.cpp
    NetworkXmlTest.cpp
    NetworkXmlReaderContextTest.cpp
    NetworkXmlWriterContextTest.cpp
    PhaseShifterRoundTripTest.cpp
    ReactiveLimitsRoundTripTest.cpp
    RegulatingTerminalRoundTripTest.cpp
    StaticVarCompensatorRoundTripTest.cpp
    TerminalRefTest.cpp
    ThreeWindingsTransformerRoundTripTest.cpp
    VscRoundTripTest.cpp
    xiidm.cpp
)

# Shared library
add_library(${EXT_NAME} SHARED ${EXT_SOURCES})
set_target_properties(${EXT_NAME} PROPERTIES SOVERSION ${IIDM_SOVERSION} VERSION ${IIDM_VERSION} OUTPUT_NAME iidm-${EXT_NAME})
target_include_directories(${EXT_NAME}
    INTERFACE
        $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(${EXT_NAME} PUBLIC iidm)

# Static library
add_library(${EXT_NAME}-static STATIC ${EXT_SOURCES})
set_target_properties(${EXT_NAME}-static PROPERTIES OUTPUT_NAME iidm-${EXT_NAME})
target_include_directories(${EXT_NAME}-static
    INTERFACE
        $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(${EXT_NAME}-static PUBLIC iidm)

# Unit tests
add_executable(unit-tests-iidm-xml ${UNIT_TEST_SOURCES})
target_compile_definitions(unit-tests-iidm-xml PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(unit-tests-iidm-xml PRIVATE ${EXT_NAME} iidm-tests)

add_test(NAME iidm-xml COMMAND unit-tests-iidm-xml ${BOOST_ARGS_SEPARATOR} --resources=${CMAKE_SOURCE_DIR}/test/resources)
