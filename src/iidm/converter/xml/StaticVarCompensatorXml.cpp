/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "StaticVarCompensatorXml.hpp"

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

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

StaticVarCompensator& StaticVarCompensatorXml::readRootElementAttributes(StaticVarCompensatorAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& bMin = context.getReader().getAttributeValue<double>(B_MIN);
    const auto& bMax = context.getReader().getAttributeValue<double>(B_MAX);
    const std::string& voltageSetpointName = context.getVersion() <= IidmXmlVersion::V1_2() ? VOLTAGE_SET_POINT : VOLTAGE_SETPOINT;
    const std::string& reactivePowerSetpointName = context.getVersion() <= IidmXmlVersion::V1_2() ? REACTIVE_POWER_SET_POINT : REACTIVE_POWER_SETPOINT;
    double voltageSetpoint = context.getReader().getOptionalAttributeValue(voltageSetpointName, stdcxx::nan());
    double reactivePowerSetpoint = context.getReader().getOptionalAttributeValue(reactivePowerSetpointName, stdcxx::nan());
    const auto& regulationMode = Enum::fromString<StaticVarCompensator::RegulationMode>(context.getReader().getAttributeValue(REGULATION_MODE));
    adder.setBmin(bMin)
            .setBmax(bMax)
            .setVoltageSetpoint(voltageSetpoint)
            .setReactivePowerSetpoint(reactivePowerSetpoint)
            .setRegulationMode(regulationMode);
    readNodeOrBus(adder, context);
    StaticVarCompensator& svc = adder.add();
    readPQ(svc.getTerminal(), context.getReader());
    return svc;
}

void StaticVarCompensatorXml::readSubElements(StaticVarCompensator& svc, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(STATIC_VAR_COMPENSATOR, [this, &svc, &context]() {
        if (context.getReader().getLocalName() == REGULATING_TERMINAL) {
            IidmXmlUtil::assertMinimumVersion(STATIC_VAR_COMPENSATOR, REGULATING_TERMINAL, xml::ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_1(), context);
            const std::string& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
            const std::string& side = context.getReader().getOptionalAttributeValue(SIDE, "");
            context.addEndTask([id, side, &svc]() {
                svc.setRegulatingTerminal(stdcxx::ref<Terminal>(TerminalRefXml::readTerminalRef(svc.getNetwork(), id, side)));
            });
        } else {
            AbstractConnectableXml::readSubElements(svc, context);
        }
    });
}

void StaticVarCompensatorXml::writeRootElementAttributes(const StaticVarCompensator& svc, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(B_MIN, svc.getBmin());
    context.getWriter().writeAttribute(B_MAX, svc.getBmax());
    const std::string& voltageSetpointName = context.getVersion() <= IidmXmlVersion::V1_2() ? VOLTAGE_SET_POINT : VOLTAGE_SETPOINT;
    const std::string& reactivePowerSetpointName = context.getVersion() <= IidmXmlVersion::V1_2() ? REACTIVE_POWER_SET_POINT : REACTIVE_POWER_SETPOINT;
    context.getWriter().writeAttribute(voltageSetpointName, svc.getVoltageSetpoint());
    context.getWriter().writeAttribute(reactivePowerSetpointName, svc.getReactivePowerSetpoint());
    context.getWriter().writeAttribute(REGULATION_MODE, Enum::toString(svc.getRegulationMode()));
    writeNodeOrBus(svc.getTerminal(), context);
    writePQ(svc.getTerminal(), context.getWriter());
}

void StaticVarCompensatorXml::writeSubElements(const StaticVarCompensator& svc, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    IidmXmlUtil::assertMinimumVersionAndRunIfNotDefault(!stdcxx::areSame(svc, svc.getRegulatingTerminal().getConnectable().get()),
        STATIC_VAR_COMPENSATOR, REGULATING_TERMINAL, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_1(), context, [&svc, &context]() {
        TerminalRefXml::writeTerminalRef(svc.getRegulatingTerminal(), context, REGULATING_TERMINAL);
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

