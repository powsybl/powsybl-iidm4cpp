#
# Copyright (c) 2019, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
    stdcxx.cpp
    CastTest.cpp
    DateTimeTest.cpp
    DemangleTest.cpp
    MathTest.cpp
    MessageFormatTest.cpp
    PropertiesTest.cpp
    ReferenceTest.cpp
)

add_executable(unit-tests-stdcxx ${UNIT_TEST_SOURCES})
target_compile_definitions(unit-tests-stdcxx PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(unit-tests-stdcxx PRIVATE iidm-tests)

add_test(NAME stdcxx COMMAND unit-tests-stdcxx)
