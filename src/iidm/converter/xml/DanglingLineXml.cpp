/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DanglingLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

DanglingLineAdder DanglingLineXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newDanglingLine();
}

const DanglingLineXml& DanglingLineXml::getInstance() {
    static DanglingLineXml s_instance;
    return s_instance;
}

const char* DanglingLineXml::getRootElementName() const  {
    return DANGLING_LINE;
}

DanglingLine& DanglingLineXml::readRootElementAttributes(DanglingLineAdder& adder, NetworkXmlReaderContext& context) const  {
    const auto& p0 = context.getReader().getAttributeValue<double>(P0);
    const auto& q0 = context.getReader().getAttributeValue<double>(Q0);
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& x = context.getReader().getAttributeValue<double>(X);
    const auto& g = context.getReader().getAttributeValue<double>(G);
    const auto& b = context.getReader().getAttributeValue<double>(B);
    const std::string& ucteXnodeCode = context.getReader().getOptionalAttributeValue(UCTE_XNODE_CODE, "");
    readNodeOrBus(adder, context);
    DanglingLine& dl = adder.setP0(p0)
        .setQ0(q0)
        .setR(r)
        .setX(x)
        .setG(g)
        .setB(b)
        .setUcteXnodeCode(ucteXnodeCode)
        .add();
    readPQ(dl.getTerminal(), context.getReader());
    return dl;
}

void DanglingLineXml::readSubElements(DanglingLine& line, NetworkXmlReaderContext& context) const  {
    context.getReader().readUntilEndElement(DANGLING_LINE, [this, &line, &context]() {
        if (context.getReader().getLocalName() == CURRENT_LIMITS) {
            CurrentLimitsAdder<std::nullptr_t, DanglingLine> adder = line.newCurrentLimits();
            readCurrentLimits(adder, context.getReader());
        } else {
            AbstractIdentifiableXml::readSubElements(line, context);
        }
    });
}

void DanglingLineXml::writeRootElementAttributes(const DanglingLine& line, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const  {
    context.getWriter().writeAttribute(P0, line.getP0());
    context.getWriter().writeAttribute(Q0, line.getQ0());
    context.getWriter().writeAttribute(R, line.getR());
    context.getWriter().writeAttribute(X, line.getX());
    context.getWriter().writeAttribute(G, line.getG());
    context.getWriter().writeAttribute(B, line.getB());
    if (!line.getUcteXnodeCode().empty()) {
        context.getWriter().writeAttribute(UCTE_XNODE_CODE, line.getUcteXnodeCode());
    }
    writeNodeOrBus(line.getTerminal(), context);
    writePQ(line.getTerminal(), context.getWriter());
}

void DanglingLineXml::writeSubElements(const DanglingLine& line, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    if (line.getCurrentLimits()) {
        writeCurrentLimits(line.getCurrentLimits(), context.getWriter());
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

