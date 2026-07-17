/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DcLineXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

DcLineAdder DcLineXml::createAdder(Network& network) const {
    return network.newDcLine();
}

const DcLineXml& DcLineXml::getInstance() {
    static DcLineXml s_instance;
    return s_instance;
}

const char* DcLineXml::getRootElementName() const {
    return DC_LINE;
}

DcLine& DcLineXml::readRootElementAttributes(DcLineAdder& adder, Network& /*network*/, NetworkXmlReaderContext& context) const {
    const auto& dcNode1Id = context.getReader().getAttributeValue(toString(DC_NODE, 1));
    const auto& dcNode2Id = context.getReader().getAttributeValue(toString(DC_NODE, 2));
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& connected1 = context.getReader().getAttributeValue<bool>(toString(CONNECTED, 1));
    const auto& connected2 = context.getReader().getAttributeValue<bool>(toString(CONNECTED, 2));

    DcLine& dcLine = adder.setDcNode1(dcNode1Id)
                            .setConnected1(connected1)
                            .setDcNode2(dcNode2Id)
                            .setConnected2(connected2)
                            .setR(r)
                            .add();

    readPI(dcLine.getDcTerminal1(), context.getReader());
    readPI(dcLine.getDcTerminal2(), context.getReader());

    return dcLine;
}

void DcLineXml::readSubElements(DcLine& dcLine, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(DC_LINE, [this, &dcLine, &context]() {
        AbstractSimpleIdentifiableXml::readSubElements(dcLine, context);
    });
}

void DcLineXml::writeRootElementAttributes(const DcLine& dcLine, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    const auto& dcTerminal1 = dcLine.getDcTerminal1();
    const auto& dcTerminal2 = dcLine.getDcTerminal2();

    context.getWriter().writeAttribute(toString(DC_NODE, 1), dcTerminal1.getDcNode().getId());
    context.getWriter().writeAttribute(toString(DC_NODE, 2), dcTerminal2.getDcNode().getId());
    context.getWriter().writeAttribute(R, dcLine.getR());
    context.getWriter().writeAttribute(toString(CONNECTED, 1), dcTerminal1.isConnected());
    context.getWriter().writeAttribute(toString(CONNECTED, 2), dcTerminal2.isConnected());

    writePI(dcTerminal1, context.getWriter());
    writePI(dcTerminal2, context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

