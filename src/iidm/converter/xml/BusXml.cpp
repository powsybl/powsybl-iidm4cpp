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
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_8(), context.getVersion(), [&context, &b]() {
        const auto& fictP0 = context.getReader().getOptionalAttributeValue(FICTITIOUS_P0, stdcxx::nan());
        const auto& fictQ0 = context.getReader().getOptionalAttributeValue(FICTITIOUS_Q0, stdcxx::nan());
        b.setFictitiousP0(fictP0).setFictitiousQ0(fictQ0);
    });
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
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_8(), context.getVersion(), [&context, &bus]() {
        context.getWriter().writeOptionalAttribute(FICTITIOUS_P0, bus.getFictitiousP0(), 0.0);
        context.getWriter().writeOptionalAttribute(FICTITIOUS_Q0, bus.getFictitiousQ0(), 0.0);
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

