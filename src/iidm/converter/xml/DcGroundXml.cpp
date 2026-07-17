/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DcGroundXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

DcGroundAdder DcGroundXml::createAdder(Network& network) const {
    return network.newDcGround();
}

const DcGroundXml& DcGroundXml::getInstance() {
    static DcGroundXml s_instance;
    return s_instance;
}

const char* DcGroundXml::getRootElementName() const {
    return DC_GROUND;
}

DcGround& DcGroundXml::readRootElementAttributes(DcGroundAdder& adder, Network& /*network*/, NetworkXmlReaderContext& context) const {
    const auto& dcNodeId = context.getReader().getAttributeValue(DC_NODE);
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& connected = context.getReader().getAttributeValue<bool>(CONNECTED);

    DcGround& dcGround = adder.setDcNode(dcNodeId)
                                .setR(r)
                                .setConnected(connected)
                                .add();

    readPI(dcGround.getDcTerminal(), context.getReader());

    return dcGround;
}

void DcGroundXml::readSubElements(DcGround& dcGround, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(DC_GROUND, [this, &dcGround, &context]() {
        AbstractSimpleIdentifiableXml::readSubElements(dcGround, context);
    });
}

void DcGroundXml::writeRootElementAttributes(const DcGround& dcGround, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    const auto& dcTerminal = dcGround.getDcTerminal();

    context.getWriter().writeAttribute(DC_NODE, dcTerminal.getDcNode().getId());
    context.getWriter().writeAttribute(R, dcGround.getR());
    context.getWriter().writeAttribute(CONNECTED, dcTerminal.isConnected());

    writePI(dcTerminal, context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

