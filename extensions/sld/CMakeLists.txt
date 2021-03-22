#
# Copyright (c) 2020, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(EXT_NAME ext-sld)

set(EXT_SOURCES
    src/BusbarSectionPosition.cpp
    src/BusbarSectionPositionAdder.cpp
    src/BusbarSectionPositionXmlSerializer.cpp
    src/ConnectablePosition.cpp
    src/ConnectablePositionAdder.cpp
    src/ConnectablePositionXmlSerializer.cpp
    src/Sld.cpp
)

set(UNIT_TEST_SOURCES
    test/BusbarSectionPositionTest.cpp
    test/ConnectablePositionTest.cpp
    test/sld.cpp
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
