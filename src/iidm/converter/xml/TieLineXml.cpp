/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TieLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

TieLineAdder TieLineXml::createAdder(Network& network) const {
    return network.newTieLine();
}

const TieLineXml& TieLineXml::getInstance() {
    static TieLineXml s_instance;
    return s_instance;
}

const char* TieLineXml::getRootElementName() const  {
    return TIE_LINE;
}

bool TieLineXml::hasSubElements(const TieLine& line) const  {
    return line.getCurrentLimits1() || line.getCurrentLimits2();
}

void TieLineXml::readHalf(TieLineAdder& adder, const NetworkXmlReaderContext& context, int side) {
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(toString(ID_, side)));
    const std::string& name = context.getAnonymizer().deanonymizeString(context.getReader().getOptionalAttributeValue(toString(NAME_ , side), ""));
    const auto& r = context.getReader().getAttributeValue<double>(toString(R_, side));
    const auto& x = context.getReader().getAttributeValue<double>(toString(X_, side));
    const auto& g1 = context.getReader().getAttributeValue<double>(toString(G1_, side));
    const auto& b1 = context.getReader().getAttributeValue<double>(toString(B1_, side));
    const auto& g2 = context.getReader().getAttributeValue<double>(toString(G2_, side));
    const auto& b2 = context.getReader().getAttributeValue<double>(toString(B2_, side));
    const auto& xnodeP = context.getReader().getAttributeValue<double>(toString(XNODE_P_, side));
    const auto& xnodeQ = context.getReader().getAttributeValue<double>(toString(XNODE_Q_, side));
    adder.setId(id)
        .setName(name)
        .setR(r)
        .setX(x)
        .setG1(g1)
        .setB1(b1)
        .setG2(g2)
        .setB2(b2)
        .setXnodeP(xnodeP)
        .setXnodeQ(xnodeQ);
}

TieLine& TieLineXml::readRootElementAttributes(TieLineAdder& adder, NetworkXmlReaderContext& context) const  {
    readHalf(adder.line1(), context, 1);
    readHalf(adder.line2(), context, 2);
    readNodeOrBus(adder, context);
    const std::string& ucteXnodeCode = context.getReader().getOptionalAttributeValue(UCTE_XNODE_CODE, "");
    TieLine& tl  = adder.setUcteXnodeCode(ucteXnodeCode).add();
    readPQ(1, tl.getTerminal1(), context.getReader());
    readPQ(2, tl.getTerminal2(), context.getReader());
    return tl;
}

void TieLineXml::readSubElements(TieLine& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TIE_LINE, [this, &line, &context]() {
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

void TieLineXml::writeHalf(const TieLine::HalfLine& halfLine, NetworkXmlWriterContext& context, int side) {
    context.getWriter().writeAttribute(toString(ID_, side), context.getAnonymizer().anonymizeString(halfLine.getId()));
    if (halfLine.getId() != halfLine.getName()) {
        context.getWriter().writeAttribute(toString(NAME_, side), context.getAnonymizer().anonymizeString(halfLine.getName()));
    }
    context.getWriter().writeAttribute(toString(R_, side), halfLine.getR());
    context.getWriter().writeAttribute(toString(X_, side), halfLine.getX());
    context.getWriter().writeAttribute(toString(G1_, side), halfLine.getG1());
    context.getWriter().writeAttribute(toString(B1_, side), halfLine.getB1());
    context.getWriter().writeAttribute(toString(G2_, side), halfLine.getG2());
    context.getWriter().writeAttribute(toString(B2_, side), halfLine.getB2());
    context.getWriter().writeAttribute(toString(XNODE_P_, side), halfLine.getXnodeP());
    context.getWriter().writeAttribute(toString(XNODE_Q_, side), halfLine.getXnodeQ());
}

void TieLineXml::writeRootElementAttributes(const TieLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(UCTE_XNODE_CODE, line.getUcteXnodeCode());
    writeNodeOrBus(1, line.getTerminal1(), context);
    writeNodeOrBus(2, line.getTerminal2(), context);
    if (context.getOptions().isWithBranchSV()) {
        writePQ(1, line.getTerminal1(), context.getWriter());
        writePQ(2, line.getTerminal2(), context.getWriter());
    }
    writeHalf(line.getHalf1(), context, 1);
    writeHalf(line.getHalf2(), context, 2);
}

void TieLineXml::writeSubElements(const TieLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
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

