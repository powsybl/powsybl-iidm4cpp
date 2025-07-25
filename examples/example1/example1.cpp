/**
 * @internal
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * @endinternal
 */

#include <iostream>

#include <libxml/parser.h>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Network.hpp>

/**
 * @example example1.cpp
 * This is an example of how to use the powsybl::iidm::Network class.
 */
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input-file> <output-file>" << '\n' << std::flush;
        return EXIT_FAILURE;
    }

    try {
        xmlInitParser();

        powsybl::iidm::Network network = powsybl::iidm::Network::readXml(argv[1]);

        powsybl::iidm::Network::writeXml(argv[2], network);

        xmlCleanupParser();

    } catch (const powsybl::PowsyblException& e) {
        std::cout << e.what() << '\n' << std::flush;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
