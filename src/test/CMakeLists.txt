#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_TEST_SOURCES
    CommandLine.cpp
    ExtensionFixture.cpp
    ResourceFixture.cpp
    XmlFixture.cpp

    converter/RoundTrip.cpp
)

if (BUILD_SHARED_LIBS)
    # Shared library
    add_library(iidm-tests SHARED ${IIDM_TEST_SOURCES})
    target_include_directories(iidm-tests
        INTERFACE
            $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
        PRIVATE
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
    target_link_libraries(iidm-tests PUBLIC iidm Boost::filesystem Boost::program_options Boost::system Boost::unit_test_framework PRIVATE LibXml2::LibXml2)

    # Installation
    install(TARGETS iidm-tests
        EXPORT iidm-targets
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    )
endif ()

if (BUILD_STATIC_LIBS)
    # Static library
    add_library(iidm-tests-static STATIC ${IIDM_TEST_SOURCES})
    target_include_directories(iidm-tests-static
        INTERFACE
            $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )
    target_link_libraries(iidm-tests-static PUBLIC iidm-static Boost::filesystem Boost::program_options Boost::system Boost::unit_test_framework PRIVATE LibXml2::LibXml2)

    # Installation
    install(TARGETS iidm-tests-static
        EXPORT iidm-targets
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    )
endif ()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/powsybl/test
    DESTINATION ${INSTALL_INCLUDE_DIR}/powsybl
)
