/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VscConverterStationXml.hpp"

#include "ReactiveLimitsXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

VscConverterStationAdder VscConverterStationXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newVscConverterStation();
}

const VscConverterStationXml& VscConverterStationXml::getInstance() {
    static VscConverterStationXml s_instance;
    return s_instance;
}

const char* VscConverterStationXml::getRootElementName() const {
    return VSC_CONVERTER_STATION;
}

bool VscConverterStationXml::hasSubElements(const VscConverterStation& /*converterStation*/) const {
    return true;
}

VscConverterStation& VscConverterStationXml::readRootElementAttributes(VscConverterStationAdder& adder, const NetworkXmlReaderContext& context) const {
    auto voltageRegulatorOn = context.getReader().getAttributeValue<bool>(VOLTAGE_REGULATOR_ON);
    auto lossFactor = context.getReader().getAttributeValue<double>(LOSS_FACTOR);
    double voltageSetpoint = context.getReader().getOptionalAttributeValue(VOLTAGE_SETPOINT, stdcxx::nan());
    double reactivePowerSetpoint = context.getReader().getOptionalAttributeValue(REACTIVE_POWER_SETPOINT, stdcxx::nan());
    readNodeOrBus(adder, context);
    VscConverterStation& converterStation = adder
        .setLossFactor(lossFactor)
        .setVoltageRegulatorOn(voltageRegulatorOn)
        .setVoltageSetpoint(voltageSetpoint)
        .setReactivePowerSetpoint(reactivePowerSetpoint)
        .add();
    readPQ(boost::optional<int>(), converterStation.getTerminal(), context.getReader());
    return converterStation;
}

void VscConverterStationXml::readSubElements(VscConverterStation& converterStation, const NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(VSC_CONVERTER_STATION, [this, &converterStation, &context]() {
        if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
            context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            ReactiveLimitsXml::getInstance().read(converterStation, context);
        } else {
            AbstractIdentifiableXml::readSubElements(converterStation, context);
        }
    });
}

void VscConverterStationXml::writeRootElementAttributes(const VscConverterStation& converterStation, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(VOLTAGE_REGULATOR_ON, converterStation.isVoltageRegulatorOn());
    context.getWriter().writeAttribute(LOSS_FACTOR, converterStation.getLossFactor());
    context.getWriter().writeAttribute(VOLTAGE_SETPOINT, converterStation.getVoltageSetpoint());
    context.getWriter().writeAttribute(REACTIVE_POWER_SETPOINT, converterStation.getReactivePowerSetpoint());
    writeNodeOrBus(boost::optional<int>(), converterStation.getTerminal(), context);
    writePQ(boost::optional<int>(), converterStation.getTerminal(), context.getWriter());
}

void VscConverterStationXml::writeSubElements(const VscConverterStation& converterStation, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    ReactiveLimitsXml::getInstance().write(converterStation, context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

