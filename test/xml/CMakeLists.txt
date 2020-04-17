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
target_link_libraries(unit-tests-xml PRIVATE iidm-tests)

add_test(NAME xml COMMAND unit-tests-xml)
