#
# Copyright (c) 2019, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    stdcxx.cpp
    DateTimeTest.cpp
    DemangleTest.cpp
    )

add_executable(unit-tests-stdcxx ${UNIT_TEST_SOURCES})
target_link_libraries(unit-tests-stdcxx PRIVATE iidm-tests Boost::disable_autolinking Boost::dynamic_linking Boost::unit_test_framework)

add_test(NAME stdcxx COMMAND unit-tests-stdcxx)
set_tests_properties(stdcxx PROPERTIES ENVIRONMENT "PATH=${WINDOWSPATH}")