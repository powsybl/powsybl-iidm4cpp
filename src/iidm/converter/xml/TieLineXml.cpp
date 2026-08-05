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

BoundaryLine& TieLineXml::readBoundaryLine(BoundaryLineAdder& adder, const NetworkXmlReaderContext& context, int side) {
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
        .setG(g1 + g2)
        .setB(b1 + b2)
        .setP0(0.0)
        .setQ0(0.0);

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_10(), context.getVersion(), [&context, &adder](){
        const std::string& ucteXnodeCode = context.getReader().getOptionalAttributeValue(UCTE_XNODE_CODE, "");
        adder.setPairingKey(ucteXnodeCode);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &adder](){
        const std::string& pairingKey = context.getReader().getOptionalAttributeValue(PAIRING_KEY, "");
        adder.setPairingKey(pairingKey);
    });

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &side, &adder]() {
        bool fictitious = context.getReader().getOptionalAttributeValue(toString(FICTITIOUS_, side), false);
        adder.setFictitious(fictitious);
    });

    return adder.add();
}

BoundaryLineAdder TieLineXml::readVlAndNodeOrBus(const NetworkXmlReaderContext& context, Network& network, int side){
    const std::string& vlId = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(toString(VOLTAGE_LEVEL_ID, side)));
    VoltageLevel& vl = network.getVoltageLevel(vlId);
    BoundaryLineAdder adderBl = vl.newBoundaryLine();
    readNodeOrBus(adderBl, std::to_string(side), context);
    return adderBl;
}

TieLine& TieLineXml::readRootElementAttributes(TieLineAdder& adder, Network& network, NetworkXmlReaderContext& context) const  {

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [&adder, &network, &context](){
        BoundaryLineAdder adderBl1 = readVlAndNodeOrBus(context, network, 1);
        BoundaryLineAdder adderBl2 = readVlAndNodeOrBus(context, network, 2);
        BoundaryLine& bl1 = readBoundaryLine(adderBl1, context, 1);
        BoundaryLine& bl2 = readBoundaryLine(adderBl2, context, 2);

        adder.setBoundaryLine1(bl1.getId())
             .setBoundaryLine2(bl2.getId());
    });
    IidmXmlUtil::runInBetweenVersions(IidmXmlVersion::V1_10(), IidmXmlVersion::V1_15(), context.getVersion(), [&adder, &context](){
        const std::string& dl1Id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(DANGLING_LINE_ID1));
        const std::string& dl2Id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(DANGLING_LINE_ID2));

        adder.setBoundaryLine1(dl1Id).setBoundaryLine2(dl2Id);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_16(), context.getVersion(), [&adder, &context](){
        const std::string& bl1Id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(BOUNDARY_LINE_ID1));
        const std::string& bl2Id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(BOUNDARY_LINE_ID2));

        adder.setBoundaryLine1(bl1Id).setBoundaryLine2(bl2Id);
    });

    TieLine& tl = adder.add();

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &tl]() {
        double half1BoundaryP = context.getReader().getOptionalAttributeValue(toString(XNODE_P_, 1), stdcxx::nan());
        double half2BoundaryP = context.getReader().getOptionalAttributeValue(toString(XNODE_P_, 2), stdcxx::nan());
        double half1BoundaryQ = context.getReader().getOptionalAttributeValue(toString(XNODE_Q_, 1), stdcxx::nan());
        double half2BoundaryQ = context.getReader().getOptionalAttributeValue(toString(XNODE_Q_, 1), stdcxx::nan());
        checkBoundaryValue(half1BoundaryP, tl.getBoundaryLine1().getBoundary().getP(), toString(XNODE_P_, 1), tl.getId());
        checkBoundaryValue(half2BoundaryP, tl.getBoundaryLine2().getBoundary().getP(), toString(XNODE_P_, 2), tl.getId());
        checkBoundaryValue(half1BoundaryQ, tl.getBoundaryLine1().getBoundary().getQ(), toString(XNODE_Q_, 1), tl.getId());
        checkBoundaryValue(half2BoundaryQ, tl.getBoundaryLine2().getBoundary().getQ(), toString(XNODE_P_, 2), tl.getId());
    });

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [&context, &tl]() {
            double p1 = context.getReader().getOptionalAttributeValue("p1", stdcxx::nan());
            double q1 = context.getReader().getOptionalAttributeValue("q1", stdcxx::nan());
            double p2 = context.getReader().getOptionalAttributeValue("p2", stdcxx::nan());
            double q2 = context.getReader().getOptionalAttributeValue("q2", stdcxx::nan());
            tl.getBoundaryLine1().getTerminal().setP(p1).setQ(q1);
            tl.getBoundaryLine2().getTerminal().setP(p2).setQ(q2);
        });

    return tl;
}

void TieLineXml::readSubElements(TieLine& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(TIE_LINE, [this, &line, &context]() {
        if (context.getReader().getLocalName() == LIMITS_GROUP_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), LIMITS_GROUP_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_12(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &line]() {
                readLoadingLimitsGroup(context, LIMITS_GROUP_1, line.getBoundaryLine1());
            });
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readActivePowerLimits(line.getOrCreateSelectedOperationalLimitsGroup1().newActivePowerLimits(), context, 1);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_1) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), APPARENT_POWER_LIMITS_1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readApparentPowerLimits(line.getOrCreateSelectedOperationalLimitsGroup1().newApparentPowerLimits(), context, 1);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS1) {
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), CURRENT_LIMITS1, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            readCurrentLimits(line.getOrCreateSelectedOperationalLimitsGroup1().newCurrentLimits(), context, 1);
        } else if(context.getReader().getLocalName() == LIMITS_GROUP_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), LIMITS_GROUP_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_12(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &line]() {
                readLoadingLimitsGroup(context, LIMITS_GROUP_2, line.getBoundaryLine2());
            });
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), ACTIVE_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readActivePowerLimits(line.getOrCreateSelectedOperationalLimitsGroup2().newActivePowerLimits(), context, 2);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS_2) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), APPARENT_POWER_LIMITS_2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &line]() {
                readApparentPowerLimits(line.getOrCreateSelectedOperationalLimitsGroup2().newApparentPowerLimits(), context, 2);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS2) {
            IidmXmlUtil::assertMaximumVersion(getRootElementName(), CURRENT_LIMITS2, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_9(), context);
            readCurrentLimits(line.getOrCreateSelectedOperationalLimitsGroup2().newCurrentLimits(), context, 2);
        } else {
            AbstractSimpleIdentifiableXml::readSubElements(line, context);
        }
    });
}

void TieLineXml::writeBoundaryLine(const BoundaryLine& bl, NetworkXmlWriterContext& context, int side) {
    const Boundary& boundary = bl.getBoundary();
    context.getWriter().writeAttribute(toString(ID_, side), context.getAnonymizer().anonymizeString(bl.getId()));
    if(!bl.getOptionalName().empty()) {
        context.getWriter().writeAttribute(toString(NAME_, side), context.getAnonymizer().anonymizeString(bl.getOptionalName()));
    }
    context.getWriter().writeAttribute(toString(R_, side), bl.getR());
    context.getWriter().writeAttribute(toString(X_, side), bl.getX());
    context.getWriter().writeAttribute(toString(G1_, side), bl.getG() / 2 );
    context.getWriter().writeAttribute(toString(B1_, side), bl.getB() / 2);
    context.getWriter().writeAttribute(toString(G2_, side), bl.getG() / 2);
    context.getWriter().writeAttribute(toString(B2_, side), bl.getB() / 2);
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_4(), context.getVersion(), [&context, &side, &boundary]() {
        context.getWriter().writeAttribute(toString(XNODE_P_, side), boundary.getP());
        context.getWriter().writeAttribute(toString(XNODE_Q_, side), boundary.getQ());
    });

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &side, &bl]() {
        context.getWriter().writeOptionalAttribute(toString(FICTITIOUS_, side), bl.isFictitious(), false);
    });
}

void TieLineXml::writeRootElementAttributes(const TieLine& tl, const Network& /*network*/, NetworkXmlWriterContext& context) const {

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_16(), context.getVersion(), [&tl, &context](){
        context.getWriter().writeAttribute(BOUNDARY_LINE_ID1, context.getAnonymizer().anonymizeString(tl.getBoundaryLine1().getId()));
        context.getWriter().writeAttribute(BOUNDARY_LINE_ID2, context.getAnonymizer().anonymizeString(tl.getBoundaryLine2().getId()));
    });
    IidmXmlUtil::runInBetweenVersions(IidmXmlVersion::V1_10(), IidmXmlVersion::V1_15(), context.getVersion(), [&tl, &context](){
        context.getWriter().writeAttribute(DANGLING_LINE_ID1, context.getAnonymizer().anonymizeString(tl.getBoundaryLine1().getId()));
        context.getWriter().writeAttribute(DANGLING_LINE_ID2, context.getAnonymizer().anonymizeString(tl.getBoundaryLine2().getId()));
    });

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [this, &tl, &context](){
        if(!tl.getPairingKey().empty()) {
            context.getWriter().writeAttribute(UCTE_XNODE_CODE, tl.getPairingKey());
        }
        writeNodeOrBus(tl.getBoundaryLine1().getTerminal(), context, 1);
        writeNodeOrBus(tl.getBoundaryLine2().getTerminal(), context, 2);
        if (context.getOptions().isWithBranchSV()) {
            writePQ(tl.getBoundaryLine1().getTerminal(), context.getWriter(), 1);
            writePQ(tl.getBoundaryLine2().getTerminal(), context.getWriter(), 2);
        }
        writeBoundaryLine(tl.getBoundaryLine1(), context, 1);
        writeBoundaryLine(tl.getBoundaryLine2(), context, 2);
    });
}

void TieLineXml::writeSubElements(const TieLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_9(), context.getVersion(), [this, &line, &context](){

        writeLimits(context, getRootElementName(), line.getSelectedOperationalLimitsGroup1(), line.getOperationalLimitsGroups1(), 1);
        writeLimits(context, getRootElementName(), line.getSelectedOperationalLimitsGroup2(), line.getOperationalLimitsGroups2(), 2);

    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

