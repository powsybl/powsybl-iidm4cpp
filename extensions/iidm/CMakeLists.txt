#
# Copyright (c) 2020, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(EXT_NAME ext-iidm)

set(EXT_SOURCES
    src/ActivePowerControl.cpp
    src/ActivePowerControlAdder.cpp
    src/ActivePowerControlXmlSerializer.cpp
    src/CoordinatedReactiveControl.cpp
    src/CoordinatedReactiveControlAdder.cpp
    src/CoordinatedReactiveControlXmlSerializer.cpp
    src/DiscreteMeasurement.cpp
    src/DiscreteMeasurementAdder.cpp
    src/DiscreteMeasurements.cpp
    src/DiscreteMeasurementsAdder.cpp
    src/DiscreteMeasurementsXmlSerializer.cpp
    src/DiscreteMeasurementValidationUtil.cpp
    src/HvdcAngleDroopActivePowerControl.cpp
    src/HvdcAngleDroopActivePowerControlAdder.cpp
    src/HvdcAngleDroopActivePowerControlXmlSerializer.cpp
    src/HvdcOperatorActivePowerRange.cpp
    src/HvdcOperatorActivePowerRangeAdder.cpp
    src/HvdcOperatorActivePowerRangeXmlSerializer.cpp
    src/Iidm.cpp
    src/InjectionObservability.cpp
    src/InjectionObservabilityAdder.cpp
    src/InjectionObservabilityXmlSerializer.cpp
    src/Measurement.cpp
    src/MeasurementAdder.cpp
    src/Measurements.cpp
    src/MeasurementsAdder.cpp
    src/MeasurementsXmlSerializer.cpp
    src/MeasurementValidationUtil.cpp
    src/Observability.cpp
    src/ObservabilityQuality.cpp
    src/RemoteReactivePowerControl.cpp
    src/RemoteReactivePowerControlAdder.cpp
    src/ThreeWindingsTransformerPhaseAngleClock.cpp
    src/ThreeWindingsTransformerPhaseAngleClockAdder.cpp
    src/ThreeWindingsTransformerPhaseAngleClockXmlSerializer.cpp
    src/ThreeWindingsTransformerToBeEstimated.cpp
    src/ThreeWindingsTransformerToBeEstimatedAdder.cpp
    src/ThreeWindingsTransformerToBeEstimatedXmlSerializer.cpp
    src/TwoWindingsTransformerPhaseAngleClock.cpp
    src/TwoWindingsTransformerPhaseAngleClockAdder.cpp
    src/TwoWindingsTransformerPhaseAngleClockXmlSerializer.cpp
    src/TwoWindingsTransformerToBeEstimated.cpp
    src/TwoWindingsTransformerToBeEstimatedAdder.cpp
    src/TwoWindingsTransformerToBeEstimatedXmlSerializer.cpp
    src/VoltagePerReactivePowerControl.cpp
    src/VoltagePerReactivePowerControlAdder.cpp
    src/VoltagePerReactivePowerControlXmlSerializer.cpp
)

set(UNIT_TEST_SOURCES
    test/ActivePowerControlTest.cpp
    test/CoordinatedReactiveControlTest.cpp
    test/DiscreteMeasurementsTest.cpp
    test/HvdcAngleDroopActivePowerControlTest.cpp
    test/HvdcOperatorActivePowerRangeTest.cpp
    test/iidm.cpp
    test/InjectionObservabilityTest.cpp
    test/MeasurementsTest.cpp
    test/RemoteReactivePowerControlTest.cpp
    test/ThreeWindingsTransformerPhaseAngleClockTest.cpp
    test/ThreeWindingsTransformerToBeEstimatedTest.cpp
    test/TwoWindingsTransformerPhaseAngleClockTest.cpp
    test/TwoWindingsTransformerToBeEstimatedTest.cpp
    test/VoltagePerReactivePowerControlTest.cpp
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
target_link_libraries(${EXT_NAME}-static PUBLIC iidm-static)

if (BUILD_TESTS)
    # Unit tests
    add_executable(unit-tests-${EXT_NAME} ${UNIT_TEST_SOURCES})
    target_compile_definitions(unit-tests-${EXT_NAME} PRIVATE BOOST_TEST_DYN_LINK)
    target_link_libraries(unit-tests-${EXT_NAME} PRIVATE ${EXT_NAME} iidm-tests)

    add_test(NAME ${EXT_NAME} COMMAND unit-tests-${EXT_NAME} -- --resources=${CMAKE_CURRENT_SOURCE_DIR}/resources)
endif()

# Installation
install(TARGETS ${EXT_NAME} ${EXT_NAME}-static
    EXPORT iidm-targets
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

install(DIRECTORY include/powsybl
    DESTINATION ${INSTALL_INCLUDE_DIR}
)
