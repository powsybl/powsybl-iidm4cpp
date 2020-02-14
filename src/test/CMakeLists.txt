#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_TEST_SOURCES
    XmlFixture.cpp

    converter/RoundTrip.cpp
)

# Static library
add_library(iidm-tests STATIC ${IIDM_TEST_SOURCES})
target_include_directories(iidm-tests
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)
target_include_directories(iidm-tests SYSTEM
    PUBLIC
        ${Boost_INCLUDE_DIR}
    PRIVATE
        ${LIBXML2_INCLUDE_DIR}
)
target_link_libraries(iidm-tests
    PUBLIC Boost::filesystem Boost::program_options Boost::system
)

# Installation
install(TARGETS iidm-tests
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/powsybl/test
    DESTINATION ${INSTALL_INCLUDE_DIR}/powsybl
)
