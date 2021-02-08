/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TieLineXml.hpp"

#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

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

void TieLineXml::readHalf(TieLineAdder::HalfLineAdder adder, const NetworkXmlReaderContext& context, int side) {
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

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &side, &adder]() {
        bool fictitious = context.getReader().getOptionalAttributeValue(toString(FICTITIOUS_, side), false);
        adder.setFictitious(fictitious);
    });
    adder.add();
}

TieLine& TieLineXml::readRootElementAttributes(TieLineAdder& adder, NetworkXmlReaderContext& context) const  {
    readHalf(adder.newHalfLine1(), context, 1);
    readHalf(adder.newHalfLine2(), context, 2);
    readNodeOrBus(adder, context);
    const std::string& ucteXnodeCode = context.getReader().getOptionalAttributeValue(UCTE_XNODE_CODE, "");
    TieLine& tl = adder.setUcteXnodeCode(ucteXnodeCode).add();
    readPQ(tl.getTerminal1(), context.getReader(), 1);
    readPQ(tl.getTerminal2(), context.getReader(), 2);
    return tl;
}

void TieLineXml::readSubElements(TieLine& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TIE_LINE, [this, &line, &context]() {
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

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &side, &halfLine]() {
        context.getWriter().writeOptionalAttribute(toString(FICTITIOUS_, side), halfLine.isFictitious(), false);
    });
}

void TieLineXml::writeRootElementAttributes(const TieLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(UCTE_XNODE_CODE, line.getUcteXnodeCode());
    writeNodeOrBus(line.getTerminal1(), context, 1);
    writeNodeOrBus(line.getTerminal2(), context, 2);
    if (context.getOptions().isWithBranchSV()) {
        writePQ(line.getTerminal1(), context.getWriter(), 1);
        writePQ(line.getTerminal2(), context.getWriter(), 2);
    }
    writeHalf(line.getHalf1(), context, 1);
    writeHalf(line.getHalf2(), context, 2);
}

void TieLineXml::writeSubElements(const TieLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
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

