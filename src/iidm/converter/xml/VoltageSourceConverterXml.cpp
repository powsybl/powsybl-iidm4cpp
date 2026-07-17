/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VoltageSourceConverterXml.hpp"

#include "ReactiveLimitsXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

const VoltageSourceConverterXml& VoltageSourceConverterXml::getInstance() {
    static VoltageSourceConverterXml s_instance;
    return s_instance;
}

VoltageSourceConverterAdder VoltageSourceConverterXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newVoltageSourceConverter();
}

const char* VoltageSourceConverterXml::getRootElementName() const {
    return VOLTAGE_SOURCE_CONVERTER;
}

VoltageSourceConverter& VoltageSourceConverterXml::readRootElementAttributes(VoltageSourceConverterAdder& adder, VoltageLevel& /*voltageLevel*/, NetworkXmlReaderContext& context) const {
    readRootElementCommonAttributes(adder, context);

    const auto& voltageRegulatorOn = context.getReader().getAttributeValue<bool>(VOLTAGE_REGULATOR_ON);

    const double& voltageSetpoint = context.getReader().getOptionalAttributeValue(VOLTAGE_SETPOINT, stdcxx::nan());
    const double& reactivePowerSetpoint = context.getReader().getOptionalAttributeValue(REACTIVE_POWER_SETPOINT, stdcxx::nan());

    VoltageSourceConverter& vsc = adder.setVoltageRegulatorOn(voltageRegulatorOn)
                                        .setVoltageSetpoint(voltageSetpoint)
                                        .setReactivePowerSetpoint(reactivePowerSetpoint)
                                        .add();

    readRootElementPqiAttributes(vsc, context);
    return vsc;
}

void VoltageSourceConverterXml::writeRootElementAttributes(const VoltageSourceConverter& vsc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    AbstractAcDcConverterXml<VoltageSourceConverter,VoltageSourceConverterAdder>::writeRootElementAttributes(vsc, voltageLevel, context);
    context.getWriter().writeAttribute(VOLTAGE_REGULATOR_ON, vsc.isVoltageRegulatorOn());
    context.getWriter().writeOptionalAttribute(VOLTAGE_SETPOINT, vsc.getVoltageSetpoint());
    context.getWriter().writeOptionalAttribute(REACTIVE_POWER_SETPOINT, vsc.getReactivePowerSetpoint());
    writeRootElementPqiAttributes(vsc, context);
}

void VoltageSourceConverterXml::readSubElements(VoltageSourceConverter& vsc, NetworkXmlReaderContext& context) const {

    context.getReader().readUntilEndElement(VOLTAGE_SOURCE_CONVERTER , [this, &vsc, &context]() {
        if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
            context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            ReactiveLimitsXml::getInstance().read(vsc, context);
        } else {
            AbstractAcDcConverterXml<VoltageSourceConverter, VoltageSourceConverterAdder>::readSubElements(vsc, context);
        }
    });

}

void VoltageSourceConverterXml::writeSubElements(const VoltageSourceConverter& vsc, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    AbstractAcDcConverterXml<VoltageSourceConverter, VoltageSourceConverterAdder>::writeSubElements(vsc, voltageLevel, context);
    ReactiveLimitsXml::getInstance().write(vsc, context);
}


}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

