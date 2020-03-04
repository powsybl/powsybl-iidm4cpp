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
    logging.cpp
    MessageFormatTest.cpp
    NoopLoggerTest.cpp
)

add_executable(unit-tests-logging ${UNIT_TEST_SOURCES})
target_compile_definitions(unit-tests-logging PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(unit-tests-logging PRIVATE iidm-tests Boost::unit_test_framework)
target_link_directories(unit-tests-logging PRIVATE ${Boost_LIBRARY_DIRS})

add_test(NAME logging COMMAND unit-tests-logging)
