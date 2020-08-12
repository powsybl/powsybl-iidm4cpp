/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NodeBreakerViewSwitchXml.hpp"

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

VoltageLevel::NodeBreakerView::SwitchAdder NodeBreakerViewSwitchXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.getNodeBreakerView().newSwitch();
}

const NodeBreakerViewSwitchXml& NodeBreakerViewSwitchXml::getInstance() {
    static NodeBreakerViewSwitchXml s_instance;
    return s_instance;
}

Switch& NodeBreakerViewSwitchXml::readRootElementAttributes(VoltageLevel::NodeBreakerView::SwitchAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& open = context.getReader().getAttributeValue<bool>(OPEN);
    const auto& kind = Enum::fromString<SwitchKind>(context.getReader().getAttributeValue(KIND));
    const auto& retained = context.getReader().getAttributeValue<bool>(RETAINED);
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&context, &adder]() {
        bool fictitious = context.getReader().getOptionalAttributeValue(FICTITIOUS, false);
        adder.setFictitious(fictitious);
    });
    const auto& node1 = context.getReader().getAttributeValue<unsigned long>(NODE1);
    const auto& node2 = context.getReader().getAttributeValue<unsigned long>(NODE2);
    return adder.setKind(kind)
        .setRetained(retained)
        .setOpen(open)
        .setNode1(node1)
        .setNode2(node2)
        .add();
}

void NodeBreakerViewSwitchXml::writeRootElementAttributes(const Switch& sw, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    AbstractSwitchXml::writeRootElementAttributes(sw, voltageLevel, context);
    const VoltageLevel::NodeBreakerView& view = voltageLevel.getNodeBreakerView();
    context.getWriter().writeAttribute(NODE1, view.getNode1(sw.getId()));
    context.getWriter().writeAttribute(NODE2, view.getNode2(sw.getId()));
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

