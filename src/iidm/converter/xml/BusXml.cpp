/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BusAdder BusXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.getBusBreakerView().newBus();
}

const BusXml& BusXml::getInstance() {
    static BusXml s_instance;
    return s_instance;
}

const char* BusXml::getRootElementName() const {
    return BUS;
}

Bus& BusXml::readRootElementAttributes(BusAdder& adder, NetworkXmlReaderContext& context) const {
    double v = context.getReader().getOptionalAttributeValue(V, stdcxx::nan());
    double angle = context.getReader().getOptionalAttributeValue(ANGLE, stdcxx::nan());
    Bus& b = adder.add();
    b.setV(v);
    b.setAngle(angle);
    return b;
}

void BusXml::readSubElements(Bus& bus, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(SUBSTATION, [this, &bus, &context]() {
        AbstractIdentifiableXml::readSubElements(bus, context);
    });
}

void BusXml::writeRootElementAttributes(const Bus& bus, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeOptionalAttribute(V, bus.getV());
    context.getWriter().writeOptionalAttribute(ANGLE, bus.getAngle());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

