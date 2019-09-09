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

bool BatteryXml::hasSubElements(const Battery& /*battery*/) const {
    return true;
}

Battery& BatteryXml::readRootElementAttributes(BatteryAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& p0 = context.getReader().getAttributeValue<double>(P0);
    const auto& q0 = context.getReader().getAttributeValue<double>(Q0);
    const auto& minP = context.getReader().getAttributeValue<double>(MIN_P);
    const auto& maxP = context.getReader().getAttributeValue<double>(MAX_P);
    readNodeOrBus(adder, context);
    Battery& battery = adder.setP0(p0)
        .setQ0(q0)
        .setMinP(minP)
        .setMaxP(maxP)
        .add();
    readPQ(boost::optional<int>(), battery.getTerminal(), context.getReader());
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
    context.getWriter().writeAttribute(P0, battery.getP0());
    context.getWriter().writeAttribute(Q0, battery.getQ0());
    context.getWriter().writeAttribute(MIN_P, battery.getMinP());
    context.getWriter().writeAttribute(MAX_P, battery.getMaxP());
    writeNodeOrBus(boost::optional<int>(), battery.getTerminal(), context);
    writePQ(boost::optional<int>(), battery.getTerminal(), context.getWriter());
}

void BatteryXml::writeSubElements(const Battery& battery, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    ReactiveLimitsXml::getInstance().write(battery, context);
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

