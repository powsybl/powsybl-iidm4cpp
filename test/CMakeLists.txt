#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_subdirectory(iidm)
add_subdirectory(logging)
add_subdirectory(math)
add_subdirectory(network)
add_subdirectory(stdcxx)
add_subdirectory(xml)

# Since Boost 1.60, program arguments and boost test arguments are separated by --
# https://github.com/boostorg/test/commit/d63379a9b3ad33d263d55c119681047c1d28547c
if (${Boost_VERSION} GREATER 106000)
    set(ARGS_SEPARATOR "--")
endif()

add_custom_target(tests
    COMMAND unit-tests-iidm ${ARGS_SEPARATOR} --resources=${CMAKE_CURRENT_SOURCE_DIR}/resources
    COMMAND unit-tests-logging
    COMMAND unit-tests-math
    COMMAND unit-tests-network
    COMMAND unit-tests-stdcxx
    COMMAND unit-tests-xml

    COMMENT "Running unit tests..."
)
