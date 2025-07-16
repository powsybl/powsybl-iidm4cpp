/**
 * @internal
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * @endinternal
 */

#include <iostream>

#include <libxml/parser.h>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ExtensionProviders.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlAdder.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlXmlSerializer.hpp>
#include <powsybl/network/EurostagFactory.hpp>

namespace xml = powsybl::iidm::converter::xml;
namespace ext = powsybl::iidm::extensions::iidm;

/**
 * @example example3.cpp
 * This is an example of how to work with IIDM extensions.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output-file>" << '\n' << std::flush;
        return EXIT_FAILURE;
    }

    try {
        xmlInitParser();

        powsybl::iidm::Network network = powsybl::network::EurostagFactory::createTutorial1Network();

        network.getGenerator("GEN").newExtension<ext::ActivePowerControlAdder>()
            .withDroop(4.0)
            .withParticipate(true)
            .add();

        // Register the serializer for the ActivePowerControl extension
        auto& extProvider = powsybl::iidm::ExtensionProviders<xml::ExtensionXmlSerializer>::getInstance();
        extProvider.registerExtension(stdcxx::make_unique<ext::ActivePowerControlXmlSerializer>());

        powsybl::iidm::Network::writeXml(argv[1], network);

        xmlCleanupParser();

    } catch (const powsybl::PowsyblException& e) {
        std::cout << e.what() << '\n' << std::flush;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
