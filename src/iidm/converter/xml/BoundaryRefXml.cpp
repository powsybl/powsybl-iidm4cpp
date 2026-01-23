/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BoundaryRefXml.hpp"

#include <powsybl/iidm/Boundary.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void BoundaryRefXml::readBoundaryRef(Network& network, NetworkXmlReaderContext& context, const std::function<void(Boundary&)>& endTaskTerminalConsumer) {
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
    
    context.addEndTask([&network, id, endTaskTerminalConsumer]() {
        DanglingLine& dl = network.getDanglingLine(id);
        endTaskTerminalConsumer(dl.getBoundary());
    });
}

void BoundaryRefXml::writeBoundaryRefAttributes(const Boundary& boundary, NetworkXmlWriterContext& context) {
    context.getWriter().writeAttribute(ID, context.getAnonymizer().anonymizeString(boundary.getDanglingLine().getId()));
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

