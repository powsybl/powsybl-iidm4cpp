#
# Copyright (c) 2017, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_subdirectory(iidm)
add_subdirectory(logging)
add_subdirectory(math)

add_custom_target(tests
    COMMAND unit-tests-iidm
    COMMAND unit-tests-logging
    COMMAND unit-tests-math

    COMMENT "Running unit tests..."
)
