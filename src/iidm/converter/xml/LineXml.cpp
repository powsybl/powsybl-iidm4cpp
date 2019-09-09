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

bool LineXml::hasSubElements(const Line& line) const {
    return line.getCurrentLimits1() || line.getCurrentLimits2();
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
    readPQ(1, line.getTerminal1(), context.getReader());
    readPQ(2, line.getTerminal2(), context.getReader());
    return line;
}

void LineXml::readSubElements(Line& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(LINE, [this, &line, &context]() {
        if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            CurrentLimitsAdder<Branch::Side, Branch> adder = line.newCurrentLimits1();
            readCurrentLimits(1, adder, context.getReader());
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            CurrentLimitsAdder<Branch::Side, Branch> adder = line.newCurrentLimits2();
            readCurrentLimits(2, adder, context.getReader());
        } else {
            AbstractIdentifiableXml::readSubElements(line, context);
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
    writeNodeOrBus(1, line.getTerminal1(), context);
    writeNodeOrBus(2, line.getTerminal2(), context);
    if (context.getOptions().isWithBranchSV()) {
        writePQ(1, line.getTerminal1(), context.getWriter());
        writePQ(2, line.getTerminal2(), context.getWriter());
    }
}

void LineXml::writeSubElements(const Line& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    if (line.getCurrentLimits1()) {
        writeCurrentLimits(1, line.getCurrentLimits1(), context.getWriter());
    }
    if (line.getCurrentLimits2()) {
        writeCurrentLimits(2, line.getCurrentLimits2(), context.getWriter());
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

