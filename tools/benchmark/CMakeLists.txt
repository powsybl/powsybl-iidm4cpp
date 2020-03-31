#
# Copyright (c) 2019, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

set(IIDM_BENCHMARK_SOURCES
    Benchmark.cpp
)

add_executable(iidm-benchmark ${IIDM_BENCHMARK_SOURCES})
target_link_libraries(iidm-benchmark PRIVATE iidm Boost::program_options Boost::dynamic_linking Boost::disable_autolinking)

