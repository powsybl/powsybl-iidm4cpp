/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusbarSectionXml.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/xml/XmlStreamException.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

BusbarSectionAdder BusbarSectionXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.getNodeBreakerView().newBusbarSection();
}

const BusbarSectionXml& BusbarSectionXml::getInstance() {
    static BusbarSectionXml s_instance;
    return s_instance;
}

const char* BusbarSectionXml::getRootElementName() const {
    return BUSBAR_SECTION;
}

BusbarSection& BusbarSectionXml::readRootElementAttributes(BusbarSectionAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& node = context.getReader().getAttributeValue<unsigned long>(NODE);
    BusbarSection& bbs = adder.setNode(node)
        .add();
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_0(), context.getVersion(), [&bbs, &context]() {
        double v = context.getReader().getOptionalAttributeValue(V, stdcxx::nan());
        double angle = context.getReader().getOptionalAttributeValue(ANGLE, stdcxx::nan());
        context.addEndTask([&bbs, angle, v]() {
            const auto& b = bbs.getTerminal().getBusView().getBus();
            if (b) {
                b.get().setV(v).setAngle(angle);
            }
        });
    });
    return bbs;
}

void BusbarSectionXml::writeRootElementAttributes(const BusbarSection& busbarSection, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(NODE, busbarSection.getTerminal().getNodeBreakerView().getNode());
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_0(), context.getVersion(), [&busbarSection, &context]() {
        context.getWriter().writeAttribute(V, busbarSection.getV());
        context.getWriter().writeAttribute(ANGLE, busbarSection.getAngle());
    });
}

void BusbarSectionXml::readSubElements(BusbarSection& busbarSection, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(BUSBAR_SECTION, [this, &busbarSection, &context]() {
        AbstractIdentifiableXml::readSubElements(busbarSection, context);
    });
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

