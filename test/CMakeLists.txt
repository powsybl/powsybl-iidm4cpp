#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(UNIT_TEST_SOURCES
        iidm/BusbarSectionTest.cpp
        iidm/CountryTest.cpp
        iidm/EnergySourceTest.cpp
        iidm/LoadTest.cpp
        iidm/NetworkFactory.cpp
        iidm/NetworkTest.cpp
        iidm/ObjectStoreTest.cpp
        iidm/SubstationTest.cpp
        iidm/VoltageLevelTest.cpp

        math/UndirectedGraphTest.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(unittests ${UNIT_TEST_SOURCES})
target_link_libraries(unittests pthread ${GTEST_BOTH_LIBRARIES} iidm)

add_custom_target(tests unittests DEPENDS unittests COMMENT "Running unit tests...")