/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TieLineXml.hpp"

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void TieLineXml::checkBoundaryValue(double imported, double calculated, const std::string& name, const std::string& tlId) {
    if (!std::isnan(imported) && imported != calculated) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<TieLineXml>();
        logger.info(stdcxx::format("%1% of TieLine %2% is recalculated. Its imported value is not used (imported value = %3%; calculated value = %4%)", name, tlId, imported, calculated));
    }
}

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
    adder.setId(id)
        .setName(name)
        .setR(r)
        .setX(x)
        .setG1(g1)
        .setB1(b1)
        .setG2(g2)
        .setB2(b2);

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
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &tl]() {
        double half1BoundaryP = context.getReader().getOptionalAttributeValue(toString(XNODE_P_, 1), stdcxx::nan());
        double half2BoundaryP = context.getReader().getOptionalAttributeValue(toString(XNODE_P_, 2), stdcxx::nan());
        double half1BoundaryQ = context.getReader().getOptionalAttributeValue(toString(XNODE_Q_, 1), stdcxx::nan());
        double half2BoundaryQ = context.getReader().getOptionalAttributeValue(toString(XNODE_Q_, 1), stdcxx::nan());
        context.addEndTask([&tl, half1BoundaryP, half2BoundaryP, half1BoundaryQ, half2BoundaryQ]() {
            checkBoundaryValue(half1BoundaryP, tl.getHalf1().getBoundary().getP(), toString(XNODE_P_, 1), tl.getId());
            checkBoundaryValue(half2BoundaryP, tl.getHalf2().getBoundary().getP(), toString(XNODE_P_, 2), tl.getId());
            checkBoundaryValue(half1BoundaryQ, tl.getHalf1().getBoundary().getQ(), toString(XNODE_Q_, 1), tl.getId());
            checkBoundaryValue(half2BoundaryQ, tl.getHalf2().getBoundary().getQ(), toString(XNODE_P_, 2), tl.getId());
        });
    });
    return tl;
}

void TieLineXml::readSubElements(TieLine& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TIE_LINE, [this, &line, &context]() {
        if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readActivePowerLimits(line.newActivePowerLimits1(), context.getReader(), 1);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readApparentPowerLimits(line.newApparentPowerLimits1(), context.getReader(), 1);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            readCurrentLimits(line.newCurrentLimits1(), context.getReader(), 1);
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readActivePowerLimits(line.newActivePowerLimits2(), context.getReader(), 2);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readApparentPowerLimits(line.newApparentPowerLimits2(), context.getReader(), 2);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            readCurrentLimits(line.newCurrentLimits2(), context.getReader(), 2);
        } else {
            AbstractConnectableXml::readSubElements(line, context);
        }
    });
}

void TieLineXml::writeHalf(const TieLine::HalfLine& halfLine, NetworkXmlWriterContext& context, int side) {
    const Boundary& boundary = halfLine.getBoundary();
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
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &side, &boundary]() {
        context.getWriter().writeAttribute(toString(XNODE_P_, side), boundary.getP());
        context.getWriter().writeAttribute(toString(XNODE_Q_, side), boundary.getQ());
    });

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
    if (line.getActivePowerLimits1()) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(ACTIVE_POWER_LIMITS, 1), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeActivePowerLimits(line.getActivePowerLimits1(), context.getWriter(), context.getVersion(), 1); });
    }
    if (line.getApparentPowerLimits1()) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(APPARENT_POWER_LIMITS, 1), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeApparentPowerLimits(line.getApparentPowerLimits1(), context.getWriter(), context.getVersion(), 1); });
    }
    if (line.getCurrentLimits1()) {
        writeCurrentLimits(line.getCurrentLimits1(), context.getWriter(), context.getVersion(), 1);
    }
    if (line.getActivePowerLimits2()) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(ACTIVE_POWER_LIMITS, 2), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeActivePowerLimits(line.getActivePowerLimits2(), context.getWriter(), context.getVersion(), 2); });
    }
    if (line.getApparentPowerLimits2()) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(APPARENT_POWER_LIMITS, 2), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeApparentPowerLimits(line.getApparentPowerLimits2(), context.getWriter(), context.getVersion(), 2); });
    }
    if (line.getCurrentLimits2()) {
        writeCurrentLimits(line.getCurrentLimits2(), context.getWriter(), context.getVersion(), 2);
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

