/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DanglingLineXml.hpp"

#include "ReactiveLimitsXml.hpp"

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
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &adder]() {
        const auto& voltageRegulationOnStr = context.getReader().getOptionalAttributeValue<bool>(GENERATION_VOLTAGE_REGULATION_ON);
        if (voltageRegulationOnStr.is_initialized()) {
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

void DanglingLineXml::readSubElements(DanglingLine& dl, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(DANGLING_LINE, [this, &dl, &context]() {
        if (context.getReader().getLocalName() == CURRENT_LIMITS) {
            CurrentLimitsAdder<std::nullptr_t, DanglingLine> adder = dl.newCurrentLimits();
            readCurrentLimits(adder, context.getReader());
        } else if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
                   context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            IidmXmlUtil::assertMinimumVersion(stdcxx::format("%1%.generation", DANGLING_LINE), "reactiveLimits", ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
            ReactiveLimitsXml::getInstance().read(dl.getGeneration(), context);
        } else {
            AbstractConnectableXml::readSubElements(dl, context);
        }
    });
}

void DanglingLineXml::writeRootElementAttributes(const DanglingLine& dl, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const  {
    const auto& generation = dl.getGeneration();
    double p0 = dl.getP0();
    double q0 = dl.getQ0();
    if (generation) {
        IidmXmlUtil::assertMinimumVersion(DANGLING_LINE, GENERATION, ErrorMessage::NOT_NULL_NOT_SUPPORTED, IidmXmlVersion::V1_3(), context);
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
    context.getWriter().writeAttribute(R, dl.getR());
    context.getWriter().writeAttribute(X, dl.getX());
    context.getWriter().writeAttribute(G, dl.getG());
    context.getWriter().writeAttribute(B, dl.getB());
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
    if (!dl.getUcteXnodeCode().empty()) {
        context.getWriter().writeAttribute(UCTE_XNODE_CODE, dl.getUcteXnodeCode());
    }
    writeNodeOrBus(dl.getTerminal(), context);
    writePQ(dl.getTerminal(), context.getWriter());
}

void DanglingLineXml::writeSubElements(const DanglingLine& dl, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    if (dl.getGeneration()) {
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_3(), context.getVersion(), [&context, &dl]() { ReactiveLimitsXml::getInstance().write(dl.getGeneration(), context); });
    }
    if (dl.getCurrentLimits()) {
        writeCurrentLimits(dl.getCurrentLimits(), context.getWriter(), context.getVersion());
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

