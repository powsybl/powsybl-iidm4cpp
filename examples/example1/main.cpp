/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fstream>
#include <iostream>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Network.hpp>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input-file> <output-file>" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        std::ifstream inputStream(argv[1]);
        powsybl::iidm::Network network = powsybl::iidm::Network::readXml(inputStream);

        std::ofstream outputStream(argv[2]);
        powsybl::iidm::Network::writeXml(outputStream, network);

    } catch (const powsybl::PowsyblException& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
