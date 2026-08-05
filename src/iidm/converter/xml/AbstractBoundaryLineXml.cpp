/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "AbstractBoundaryLineXml.hpp"

#include <powsybl/iidm/ActivePowerLimitsAdder.hpp>
#include <powsybl/iidm/ApparentPowerLimitsAdder.hpp>
#include <powsybl/iidm/CurrentLimitsAdder.hpp>

#include "ReactiveLimitsXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BoundaryLineAdder AbstractBoundaryLineXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newBoundaryLine();
}

BoundaryLine& AbstractBoundaryLineXml::readRootElementAttributes(BoundaryLineAdder& adder, VoltageLevel& /*voltageLevel*/, NetworkXmlReaderContext& context) const  {
    assertReadCompatibilityVersion(context);
    double p0 = context.getReader().getOptionalAttributeValue(P0, stdcxx::nan());
    double q0 = context.getReader().getOptionalAttributeValue(Q0, stdcxx::nan());
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& x = context.getReader().getAttributeValue<double>(X);
    const auto& g = context.getReader().getAttributeValue<double>(G);
    const auto& b = context.getReader().getAttributeValue<double>(B);
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &adder]() {
        const auto& voltageRegulationOnStr = context.getReader().getOptionalAttributeValue<bool>(GENERATION_VOLTAGE_REGULATION_ON);
        if (voltageRegulationOnStr.has_value()) {
            double minP = context.getReader().getOptionalAttributeValue(GENERATION_MIN_P, stdcxx::nan());
            double maxP = context.getReader().getOptionalAttributeValue(GENERATION_MAX_P, stdcxx::nan());
            bool voltageRegulationOn = *voltageRegulationOnStr;
            double targetP = context.getReader().getOptionalAttributeValue(GENERATION_TARGET_P, stdcxx::nan());
            double targetV = context.getReader().getOptionalAttributeValue(GENERATION_TARGET_V, stdcxx::nan());
            double targetQ = context.getReader().getOptionalAttributeValue(GENERATION_TARGET_Q, stdcxx::nan());
            adder.newGeneration()
                .setMinP(minP)
                .setMaxP(maxP)
                .setVoltageRegulationOn(voltageRegulationOn)
                .setTargetP(targetP)
                .setTargetV(targetV)
                .setTargetQ(targetQ)
                .add();
        }
    });
    
    readNodeOrBus(adder, context);
    adder.setP0(p0)
        .setQ0(q0)
        .setR(r)
        .setX(x)
        .setG(g)
        .setB(b);

    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_10(), context.getVersion(), [&context, &adder](){
        const std::string& ucteXnodeCode = context.getReader().getOptionalAttributeValue(UCTE_XNODE_CODE, "");
        adder.setPairingKey(ucteXnodeCode);
    });
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &adder](){
        const std::string& pairingKey = context.getReader().getOptionalAttributeValue(PAIRING_KEY, "");
        adder.setPairingKey(pairingKey);
    });
    
    BoundaryLine& bl = adder.add();
    readPQ(bl.getTerminal(), context.getReader());

    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &bl](){
        readSelectedGroupId(context, [&bl](const std::string& selectedId) {
            bl.setSelectedOperationalLimitsGroup(selectedId);
        });
    });

    return bl;
}

void AbstractBoundaryLineXml::readSubElements(BoundaryLine& bl, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(getRootElementName(), [this, &bl, &context]() {
        if (context.getReader().getLocalName() == LIMITS_GROUP) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), LIMITS_GROUP, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_12(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(), [&context, &bl]() {
                readLoadingLimitsGroup(context, LIMITS_GROUP, bl);
            });
        } else if (context.getReader().getLocalName() == ACTIVE_POWER_LIMITS) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), ACTIVE_POWER_LIMITS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &bl]() {
                readActivePowerLimits(bl.getOrCreateSelectedOperationalLimitsGroup().newActivePowerLimits(), context);
            });
        } else if (context.getReader().getLocalName() == APPARENT_POWER_LIMITS) {
            IidmXmlUtil::assertMinimumVersion(getRootElementName(), APPARENT_POWER_LIMITS, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_5(), context);
            IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_5(), context.getVersion(), [&context, &bl]() {
                readApparentPowerLimits(bl.getOrCreateSelectedOperationalLimitsGroup().newApparentPowerLimits(), context);
            });
        } else if (context.getReader().getLocalName() == CURRENT_LIMITS) {
            readCurrentLimits(bl.getOrCreateSelectedOperationalLimitsGroup().newCurrentLimits(), context);
        } else if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
                   context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            IidmXmlUtil::assertMinimumVersion(stdcxx::format("%1%.generation", getRootElementName()), "reactiveLimits", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            ReactiveLimitsXml::getInstance().read(bl.getGeneration(), context);
        } else {
            AbstractSimpleIdentifiableXml::readSubElements(bl, context);
        }
    });
}

void AbstractBoundaryLineXml::writeRootElementAttributes(const BoundaryLine& bl, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const  {
    assertWriteCompatibilityVersion(context);
    const auto& generation = bl.getGeneration();
    double p0 = bl.getP0();
    double q0 = bl.getQ0();
    if (generation) {
        IidmXmlUtil::assertMinimumVersion(getRootElementName(), GENERATION, ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
        IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&p0, &q0, &generation]() {
            if (!std::isnan(generation.get().getTargetP())) {
                p0 -= generation.get().getTargetP();
            }
            if (!std::isnan(generation.get().getTargetQ())) {
                q0 -= generation.get().getTargetQ();
            }
        });
    }
    context.getWriter().writeAttribute(P0, p0);
    context.getWriter().writeAttribute(Q0, q0);
    context.getWriter().writeAttribute(R, bl.getR());
    context.getWriter().writeAttribute(X, bl.getX());
    context.getWriter().writeAttribute(G, bl.getG());
    context.getWriter().writeAttribute(B, bl.getB());
    if (generation) {
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &generation]() {
            context.getWriter().writeAttribute(GENERATION_MIN_P, generation.get().getMinP());
            context.getWriter().writeAttribute(GENERATION_MAX_P, generation.get().getMaxP());
            context.getWriter().writeAttribute(GENERATION_VOLTAGE_REGULATION_ON, generation.get().isVoltageRegulationOn());
            context.getWriter().writeAttribute(GENERATION_TARGET_P, generation.get().getTargetP());
            context.getWriter().writeAttribute(GENERATION_TARGET_V, generation.get().getTargetV());
            context.getWriter().writeAttribute(GENERATION_TARGET_Q, generation.get().getTargetQ());
        });
    }
    if (!bl.getPairingKey().empty()) {
        IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_10(), context.getVersion(), [&context, &bl](){
            context.getWriter().writeAttribute(UCTE_XNODE_CODE, bl.getPairingKey());
        });
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_11(), context.getVersion(), [&context, &bl](){
            context.getWriter().writeAttribute(PAIRING_KEY, bl.getPairingKey());
        });
    }
    writeNodeOrBus(bl.getTerminal(), context);
    writePQ(bl.getTerminal(), context.getWriter());
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_12(), context.getVersion(),[&bl, &context](){
        writeSelectedGroupId(bl.getSelectedOperationalLimitsGroupId(), context);
    });
}

void AbstractBoundaryLineXml::writeSubElements(const BoundaryLine& bl, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    if (bl.getGeneration()) {
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &bl]() { ReactiveLimitsXml::getInstance().write(bl.getGeneration(), context); });
    }

    writeLimits(context, getRootElementName(), bl.getSelectedOperationalLimitsGroup(), bl.getOperationalLimitsGroups());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

