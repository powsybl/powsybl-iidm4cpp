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

DanglingLine& TieLineXml::readDanglingLine(DanglingLineAdder& adder, const NetworkXmlReaderContext& context, int side) {
    const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(toString(ID_, side)));
    const std::string& name = context.getAnonymizer().deanonymizeString(context.getReader().getOptionalAttributeValue(toString(NAME_ , side), ""));
    const auto& r = context.getReader().getAttributeValue<double>(toString(R_, side));
    const auto& x = context.getReader().getAttributeValue<double>(toString(X_, side));
    const auto& g1 = context.getReader().getAttributeValue<double>(toString(G1_, side));
    const auto& b1 = context.getReader().getAttributeValue<double>(toString(B1_, side));
    const auto& g2 = context.getReader().getAttributeValue<double>(toString(G2_, side));
    const auto& b2 = context.getReader().getAttributeValue<double>(toString(B2_, side));
    const std::string& ucteXnodeCode = context.getReader().getOptionalAttributeValue(UCTE_XNODE_CODE, "");
    adder.setId(id)
        .setName(name)
        .setR(r)
        .setX(x)
        .setG(g1 + g2)
        .setB(b1 + b2)
        .setP0(0.0)
        .setQ0(0.0)
        .setUcteXnodeCode(ucteXnodeCode);

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &side, &adder]() {
        bool fictitious = context.getReader().getOptionalAttributeValue(toString(FICTITIOUS_, side), false);
        adder.setFictitious(fictitious);
    });

    return adder.add();
}

DanglingLineAdder TieLineXml::readVlAndNodeOrBus(const NetworkXmlReaderContext& context, Network& network, int side){
    const std::string& vlId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(toString(VOLTAGE_LEVEL_ID, side)));
    VoltageLevel& vl = network.getVoltageLevel(vlId);
    DanglingLineAdder adderDl = vl.newDanglingLine();
    readNodeOrBus(adderDl, std::to_string(side), context);
    return adderDl;
}

TieLine& TieLineXml::readRootElementAttributes(TieLineAdder& adder, Network& network, NetworkXmlReaderContext& context) const  {

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [&adder, &network, &context](){
        DanglingLineAdder adderDl1 = readVlAndNodeOrBus(context, network, 1);
        DanglingLineAdder adderDl2 = readVlAndNodeOrBus(context, network, 2);
        DanglingLine& dl1 = readDanglingLine(adderDl1, context, 1);
        DanglingLine& dl2 = readDanglingLine(adderDl2, context, 2);

        adder.setDanglingLine1(dl1.getId())
             .setDanglingLine2(dl2.getId());
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_10(), context.getVersion(), [&adder, &context](){
        const std::string& dl1Id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(DANGLING_LINE_ID1));
        const std::string& dl2Id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(DANGLING_LINE_ID2));

        adder.setDanglingLine1(dl1Id).setDanglingLine2(dl2Id);
    });

    TieLine& tl = adder.add();

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &tl]() {
        double half1BoundaryP = context.getReader().getOptionalAttributeValue(toString(XNODE_P_, 1), stdcxx::nan());
        double half2BoundaryP = context.getReader().getOptionalAttributeValue(toString(XNODE_P_, 2), stdcxx::nan());
        double half1BoundaryQ = context.getReader().getOptionalAttributeValue(toString(XNODE_Q_, 1), stdcxx::nan());
        double half2BoundaryQ = context.getReader().getOptionalAttributeValue(toString(XNODE_Q_, 1), stdcxx::nan());
        checkBoundaryValue(half1BoundaryP, tl.getDanglingLine1().getBoundary().getP(), toString(XNODE_P_, 1), tl.getId());
        checkBoundaryValue(half2BoundaryP, tl.getDanglingLine2().getBoundary().getP(), toString(XNODE_P_, 2), tl.getId());
        checkBoundaryValue(half1BoundaryQ, tl.getDanglingLine1().getBoundary().getQ(), toString(XNODE_Q_, 1), tl.getId());
        checkBoundaryValue(half2BoundaryQ, tl.getDanglingLine2().getBoundary().getQ(), toString(XNODE_P_, 2), tl.getId());
    });

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [&context, &tl]() {
            double p1 = context.getReader().getOptionalAttributeValue("p1", stdcxx::nan());
            double q1 = context.getReader().getOptionalAttributeValue("q1", stdcxx::nan());
            double p2 = context.getReader().getOptionalAttributeValue("p2", stdcxx::nan());
            double q2 = context.getReader().getOptionalAttributeValue("q2", stdcxx::nan());
            tl.getDanglingLine1().getTerminal().setP(p1).setQ(q1);
            tl.getDanglingLine2().getTerminal().setP(p2).setQ(q2);
        });

    return tl;
}

void TieLineXml::readSubElements(TieLine& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TIE_LINE, [this, &line, &context]() {
        if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readActivePowerLimits(line.getDanglingLine1().newActivePowerLimits(), context.getReader(), 1);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), APPARENT_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readApparentPowerLimits(line.getDanglingLine1().newApparentPowerLimits(), context.getReader(), 1);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), CURRENT_LIMITS1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            readCurrentLimits(line.getDanglingLine1().newCurrentLimits(), context.getReader(), 1);
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readActivePowerLimits(line.getDanglingLine2().newActivePowerLimits(), context.getReader(), 2);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), APPARENT_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readApparentPowerLimits(line.getDanglingLine2().newApparentPowerLimits(), context.getReader(), 2);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), CURRENT_LIMITS2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            readCurrentLimits(line.getDanglingLine2().newCurrentLimits(), context.getReader(), 2);
        } else {
            AbstractSimpleIdentifiableXml::readSubElements(line, context);
        }
    });
}

void TieLineXml::writeDanglingLine(const DanglingLine& dl, NetworkXmlWriterContext& context, int side) {
    const Boundary& boundary = dl.getBoundary();
    context.getWriter().writeAttribute(toString(ID_, side), context.getAnonymizer().anonymizeString(dl.getId()));
    if(!dl.getOptionalName().empty()) {
        context.getWriter().writeAttribute(toString(NAME_, side), context.getAnonymizer().anonymizeString(dl.getOptionalName()));
    }
    context.getWriter().writeAttribute(toString(R_, side), dl.getR());
    context.getWriter().writeAttribute(toString(X_, side), dl.getX());
    context.getWriter().writeAttribute(toString(G1_, side), dl.getG() / 2 );
    context.getWriter().writeAttribute(toString(B1_, side), dl.getB() / 2);
    context.getWriter().writeAttribute(toString(G2_, side), dl.getG() / 2);
    context.getWriter().writeAttribute(toString(B2_, side), dl.getB() / 2);
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &side, &boundary]() {
        context.getWriter().writeAttribute(toString(XNODE_P_, side), boundary.getP());
        context.getWriter().writeAttribute(toString(XNODE_Q_, side), boundary.getQ());
    });

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &side, &dl]() {
        context.getWriter().writeOptionalAttribute(toString(FICTITIOUS_, side), dl.isFictitious(), false);
    });
}

void TieLineXml::writeRootElementAttributes(const TieLine& tl, const Network& /*network*/, NetworkXmlWriterContext& context) const {

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_10(), context.getVersion(), [&tl, &context](){
        context.getWriter().writeAttribute(DANGLING_LINE_ID1, context.getAnonymizer().anonymizeString(tl.getDanglingLine1().getId()));
        context.getWriter().writeAttribute(DANGLING_LINE_ID2, context.getAnonymizer().anonymizeString(tl.getDanglingLine2().getId()));
    });

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [this, &tl, &context](){
        if(!tl.getUcteXnodeCode().empty()) {
            context.getWriter().writeAttribute(UCTE_XNODE_CODE, tl.getUcteXnodeCode());
        }
        writeNodeOrBus(tl.getDanglingLine1().getTerminal(), context, 1);
        writeNodeOrBus(tl.getDanglingLine2().getTerminal(), context, 2);
        if (context.getOptions().isWithBranchSV()) {
            writePQ(tl.getDanglingLine1().getTerminal(), context.getWriter(), 1);
            writePQ(tl.getDanglingLine2().getTerminal(), context.getWriter(), 2);
        }
        writeDanglingLine(tl.getDanglingLine1(), context, 1);
        writeDanglingLine(tl.getDanglingLine2(), context, 2);
    });
}

void TieLineXml::writeSubElements(const TieLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [this, &line, &context](){
        if (line.getDanglingLine1().getActivePowerLimits()) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(ACTIVE_POWER_LIMITS, 1), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeActivePowerLimits(line.getDanglingLine1().getActivePowerLimits(), context.getWriter(), context.getVersion(), 1); });
        }
        if (line.getDanglingLine1().getApparentPowerLimits()) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(APPARENT_POWER_LIMITS, 1), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeApparentPowerLimits(line.getDanglingLine1().getApparentPowerLimits(), context.getWriter(), context.getVersion(), 1); });
        }
        if (line.getDanglingLine1().getCurrentLimits()) {
            writeCurrentLimits(line.getDanglingLine1().getCurrentLimits(), context.getWriter(), context.getVersion(), 1);
        }
        if (line.getDanglingLine2().getActivePowerLimits()) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(ACTIVE_POWER_LIMITS, 2), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeActivePowerLimits(line.getDanglingLine2().getActivePowerLimits(), context.getWriter(), context.getVersion(), 2); });
        }
        if (line.getDanglingLine2().getApparentPowerLimits()) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), toString(APPARENT_POWER_LIMITS, 2), ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&line, &context]() { writeApparentPowerLimits(line.getDanglingLine2().getApparentPowerLimits(), context.getWriter(), context.getVersion(), 2); });
        }
        if (line.getDanglingLine2().getCurrentLimits()) {
            writeCurrentLimits(line.getDanglingLine2().getCurrentLimits(), context.getWriter(), context.getVersion(), 2);
        }
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

