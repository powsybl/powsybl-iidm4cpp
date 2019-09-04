/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "StaticVarCompensatorXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

StaticVarCompensatorAdder StaticVarCompensatorXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newStaticVarCompensator();
}

const StaticVarCompensatorXml& StaticVarCompensatorXml::getInstance() {
    static StaticVarCompensatorXml s_instance;
    return s_instance;
}

const char* StaticVarCompensatorXml::getRootElementName() const {
    return STATIC_VAR_COMPENSATOR;
}

bool StaticVarCompensatorXml::hasSubElements(const StaticVarCompensator& /*svc*/) const {
    return false;
}

StaticVarCompensator& StaticVarCompensatorXml::readRootElementAttributes(StaticVarCompensatorAdder& adder, const NetworkXmlReaderContext& context) const {
    const auto& bMin = context.getReader().getAttributeValue<double>(B_MIN);
    const auto& bMax = context.getReader().getAttributeValue<double>(B_MAX);
    double voltageSetpoint = context.getReader().getOptionalAttributeValue(VOLTAGE_SET_POINT, stdcxx::nan());
    double reactivePowerSetpoint = context.getReader().getOptionalAttributeValue(REACTIVE_POWER_SET_POINT, stdcxx::nan());
    StaticVarCompensator::RegulationMode regulationMode = StaticVarCompensator::getRegulationMode(context.getReader().getAttributeValue(REGULATION_MODE));
    adder.setBmin(bMin)
            .setBmax(bMax)
            .setVoltageSetpoint(voltageSetpoint)
            .setReactivePowerSetpoint(reactivePowerSetpoint)
            .setRegulationMode(regulationMode);
    readNodeOrBus(adder, context);
    StaticVarCompensator& svc = adder.add();
    readPQ(boost::optional<int>(), svc.getTerminal(), context.getReader());
    return svc;
}

void StaticVarCompensatorXml::readSubElements(StaticVarCompensator& svc, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(STATIC_VAR_COMPENSATOR, [this, &svc, &context]() {
        AbstractConnectableXml::readSubElements(svc, context);
    });
}

void StaticVarCompensatorXml::writeRootElementAttributes(const StaticVarCompensator& svc, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(B_MIN, svc.getBmin());
    context.getWriter().writeAttribute(B_MAX, svc.getBmax());
    context.getWriter().writeAttribute(VOLTAGE_SET_POINT, svc.getVoltageSetpoint());
    context.getWriter().writeAttribute(REACTIVE_POWER_SET_POINT, svc.getReactivePowerSetpoint());
    context.getWriter().writeAttribute(REGULATION_MODE, StaticVarCompensator::getRegulationModeName(svc.getRegulationMode()));
    writeNodeOrBus(boost::optional<int>(), svc.getTerminal(), context);
    writePQ(boost::optional<int>(), svc.getTerminal(), context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

