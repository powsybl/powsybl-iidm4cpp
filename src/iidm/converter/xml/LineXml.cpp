/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

LineAdder LineXml::createAdder(Network& network) const {
    return network.newLine();
}

const LineXml& LineXml::getInstance() {
    static LineXml s_instance;
    return s_instance;
}

const char* LineXml::getRootElementName() const {
    return LINE;
}

Line& LineXml::readRootElementAttributes(LineAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& x = context.getReader().getAttributeValue<double>(X);
    const auto& g1 = context.getReader().getAttributeValue<double>(G1);
    const auto& b1 = context.getReader().getAttributeValue<double>(B1);
    const auto& g2 = context.getReader().getAttributeValue<double>(G2);
    const auto& b2 = context.getReader().getAttributeValue<double>(B2);
    adder.setR(r)
        .setX(x)
        .setG1(g1)
        .setB1(b1)
        .setG2(g2)
        .setB2(b2);
    readNodeOrBus(adder, context);
    Line& line = adder.add();
    readPQ(line.getTerminal1(), context.getReader(), 1);
    readPQ(line.getTerminal2(), context.getReader(), 2);
    return line;
}

void LineXml::readSubElements(Line& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(LINE, [this, &line, &context]() {
        if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            CurrentLimitsAdder<Branch::Side, Branch> adder = line.newCurrentLimits1();
            readCurrentLimits(adder, context.getReader(), 1);
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            CurrentLimitsAdder<Branch::Side, Branch> adder = line.newCurrentLimits2();
            readCurrentLimits(adder, context.getReader(), 2);
        } else {
            AbstractConnectableXml::readSubElements(line, context);
        }
    });
}

void LineXml::writeRootElementAttributes(const Line& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(R, line.getR());
    context.getWriter().writeAttribute(X, line.getX());
    context.getWriter().writeAttribute(G1, line.getG1());
    context.getWriter().writeAttribute(B1, line.getB1());
    context.getWriter().writeAttribute(G2, line.getG2());
    context.getWriter().writeAttribute(B2, line.getB2());
    writeNodeOrBus(line.getTerminal1(), context, 1);
    writeNodeOrBus(line.getTerminal2(), context, 2);
    if (context.getOptions().isWithBranchSV()) {
        writePQ(line.getTerminal1(), context.getWriter(), 1);
        writePQ(line.getTerminal2(), context.getWriter(), 2);
    }
}

void LineXml::writeSubElements(const Line& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    if (line.getCurrentLimits1()) {
        writeCurrentLimits(line.getCurrentLimits1(), context.getWriter(), context.getVersion(), 1);
    }
    if (line.getCurrentLimits2()) {
        writeCurrentLimits(line.getCurrentLimits2(), context.getWriter(), context.getVersion(), 2);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

