/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BatteryXml.hpp"

#include "ReactiveLimitsXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BatteryAdder BatteryXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newBattery();
}

const BatteryXml& BatteryXml::getInstance() {
    static BatteryXml s_instance;
    return s_instance;
}

const char* BatteryXml::getRootElementName() const {
    return BATTERY;
}

Battery& BatteryXml::readRootElementAttributes(BatteryAdder& adder, NetworkXmlReaderContext& context) const {
    const std::string& targetPName = context.getVersion() <= IidmXmlVersion::V1_7() ? P0 : TARGET_P;
    const std::string& targetQName = context.getVersion() <= IidmXmlVersion::V1_7() ? Q0 : TARGET_Q;
    const auto& targetP = context.getReader().getAttributeValue<double>(targetPName);
    const auto& targetQ = context.getReader().getAttributeValue<double>(targetQName);
    const auto& minP = context.getReader().getAttributeValue<double>(MIN_P);
    const auto& maxP = context.getReader().getAttributeValue<double>(MAX_P);
    readNodeOrBus(adder, context);
    Battery& battery = adder.setTargetP(targetP)
        .setTargetQ(targetQ)
        .setMinP(minP)
        .setMaxP(maxP)
        .add();
    readPQ(battery.getTerminal(), context.getReader());
    return battery;
}

void BatteryXml::readSubElements(Battery& battery, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(BATTERY, [this, &battery, &context]() {
        if (context.getReader().getLocalName() == REACTIVE_CAPABILITY_CURVE ||
            context.getReader().getLocalName() == MIN_MAX_REACTIVE_LIMITS) {
            ReactiveLimitsXml::getInstance().read(battery, context);
        } else {
            AbstractConnectableXml::readSubElements(battery, context);
        }
    });
}

void BatteryXml::writeRootElementAttributes(const Battery& battery, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    const std::string& targetPName = context.getVersion() <= IidmXmlVersion::V1_7() ? P0 : TARGET_P;
    const std::string& targetQName = context.getVersion() <= IidmXmlVersion::V1_7() ? Q0 : TARGET_Q;
    context.getWriter().writeAttribute(targetPName, battery.getTargetP());
    context.getWriter().writeAttribute(targetQName, battery.getTargetQ());
    context.getWriter().writeAttribute(MIN_P, battery.getMinP());
    context.getWriter().writeAttribute(MAX_P, battery.getMaxP());
    writeNodeOrBus(battery.getTerminal(), context);
    writePQ(battery.getTerminal(), context.getWriter());
}

void BatteryXml::writeSubElements(const Battery& battery, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    ReactiveLimitsXml::getInstance().write(battery, context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

