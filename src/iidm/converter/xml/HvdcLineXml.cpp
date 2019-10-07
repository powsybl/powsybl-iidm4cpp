/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "HvdcLineXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

HvdcLineAdder HvdcLineXml::createAdder(Network& network) const {
    return network.newHvdcLine();
}

const HvdcLineXml& HvdcLineXml::getInstance() {
    static HvdcLineXml s_instance;
    return s_instance;
}

const char* HvdcLineXml::getRootElementName() const {
    return HVDC_LINE;
}

HvdcLine& HvdcLineXml::readRootElementAttributes(HvdcLineAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& r = context.getReader().getAttributeValue<double>(R);
    const auto& nominalV = context.getReader().getAttributeValue<double>(NOMINAL_V);
    // FIXME(sebalaig) should be updated while fixing issue #43 (https://devin-source.rte-france.com/powsybl/powsybl-iidm/issues/43)
    HvdcLine::ConvertersMode convertersMode = getConvertersMode(context.getReader().getAttributeValue(CONVERTERS_MODE));
    const auto& activePowerSetpoint = context.getReader().getAttributeValue<double>(ACTIVE_POWER_SETPOINT);
    const auto& maxP = context.getReader().getAttributeValue<double>(MAX_P);
    const std::string& converterStation1 = context.getReader().getAttributeValue(CONVERTER_STATION1);
    const std::string& converterStation2 = context.getReader().getAttributeValue(CONVERTER_STATION2);
    return adder.setR(r)
        .setNominalVoltage(nominalV) // called setNominalV() in Java code
        .setConvertersMode(convertersMode)
        .setActivePowerSetpoint(activePowerSetpoint)
        .setMaxP(maxP)
        .setConverterStationId1(converterStation1)
        .setConverterStationId2(converterStation2)
        .add();
}

void HvdcLineXml::readSubElements(HvdcLine& line, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(HVDC_LINE, [this, &line, &context]() {
        AbstractIdentifiableXml::readSubElements(line, context);
    });
}

void HvdcLineXml::writeRootElementAttributes(const HvdcLine& line, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(R, line.getR());
    context.getWriter().writeAttribute(NOMINAL_V, line.getNominalVoltage()); // called HvdcLine::getNominalV in java code
    context.getWriter().writeAttribute(CONVERTERS_MODE, getConvertersModeName(line.getConvertersMode()));
    context.getWriter().writeAttribute(ACTIVE_POWER_SETPOINT, line.getActivePowerSetpoint());
    context.getWriter().writeAttribute(MAX_P, line.getMaxP());
    context.getWriter().writeAttribute(CONVERTER_STATION1, line.getConverterStation1().get().getId());
    context.getWriter().writeAttribute(CONVERTER_STATION2, line.getConverterStation2().get().getId());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

