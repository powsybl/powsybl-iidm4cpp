/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LccConverterStationXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

LccConverterStationAdder LccConverterStationXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newLccConverterStation();
}

const LccConverterStationXml& LccConverterStationXml::LccConverterStationXml::getInstance() {
    static LccConverterStationXml s_instance;
    return s_instance;
}

const char* LccConverterStationXml::getRootElementName() const {
    return LCC_CONVERTER_STATION;
}

LccConverterStation& LccConverterStationXml::readRootElementAttributes(LccConverterStationAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& lossFactor = context.getReader().getAttributeValue<double>(LOSS_FACTOR);
    const double& powerFactor = context.getReader().getOptionalAttributeValue(POWER_FACTOR, stdcxx::nan());
    readNodeOrBus(adder, context);
    LccConverterStation& converterStation = adder
        .setLossFactor(lossFactor)
        .setPowerFactor(powerFactor)
        .add();
    readPQ(converterStation.getTerminal(), context.getReader());
    return converterStation;
}

void LccConverterStationXml::readSubElements(LccConverterStation& converterStation, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(LCC_CONVERTER_STATION, [this, &converterStation, &context]() {
        AbstractConnectableXml::readSubElements(converterStation, context);
    });
}

void LccConverterStationXml::writeRootElementAttributes(const LccConverterStation& converterStation, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(LOSS_FACTOR, converterStation.getLossFactor());
    context.getWriter().writeOptionalAttribute(POWER_FACTOR, converterStation.getPowerFactor());
    writeNodeOrBus(converterStation.getTerminal(), context);
    writePQ(converterStation.getTerminal(), context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

