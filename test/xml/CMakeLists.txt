#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    xml.cpp
    XmlStreamReaderTest.cpp
    XmlStreamWriterTest.cpp)

add_executable(unit-tests-xml ${UNIT_TEST_SOURCES})
target_include_directories(unit-tests-xml
    PRIVATE Boost::boost
    PRIVATE ${LIBXML2_INCLUDE_DIR}
    PRIVATE ${CMAKE_HOME_DIRECTORY}/include
    PRIVATE ${CMAKE_HOME_DIRECTORY}/test
)
target_compile_definitions(unit-tests-xml
    PRIVATE BOOST_TEST_DYN_LINK
)
target_link_libraries(unit-tests-xml
    iidm
    ${LIBXML2_LIBRARIES}
)
