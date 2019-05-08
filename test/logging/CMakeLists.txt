#
# Copyright (c) 2018, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    ConsoleLoggerTest.cpp
    ContainerLoggerTest.cpp
    LoggerFactoryTest.cpp
    MessageFormatTest.cpp
    NoopLoggerTest.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(unit-tests-logging ${UNIT_TEST_SOURCES})
target_link_libraries(unit-tests-logging ${GTEST_BOTH_LIBRARIES} pthread iidm)
