/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "BusBreakerViewSwitchXml.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlUtil.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

VoltageLevel::BusBreakerView::SwitchAdder BusBreakerViewSwitchXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.getBusBreakerView().newSwitch();
}

const BusBreakerViewSwitchXml& BusBreakerViewSwitchXml::getInstance() {
    static BusBreakerViewSwitchXml s_instance;
    return s_instance;
}

Switch& BusBreakerViewSwitchXml::readRootElementAttributes(VoltageLevel::BusBreakerView::SwitchAdder& adder, NetworkXmlReaderContext& context) const {
    const auto& open = context.getReader().getAttributeValue<bool>(OPEN);
    IidmXmlUtil::runUntilMaximumVersion(IidmXmlVersion::V1_1(), context.getVersion(), [&context, &adder]() {
        bool fictitious = context.getReader().getOptionalAttributeValue(FICTITIOUS, false);
        adder.setFictitious(fictitious);
    });
    const std::string& bus1 = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(BUS1));
    const std::string& bus2 = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(BUS2));
    return adder.setOpen(open)
        .setBus1(bus1)
        .setBus2(bus2)
        .add();
}

void BusBreakerViewSwitchXml::writeRootElementAttributes(const Switch& sw, const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) const {
    AbstractSwitchXml::writeRootElementAttributes(sw, voltageLevel, context);
    const VoltageLevel::BusBreakerView& view = voltageLevel.getBusBreakerView();
    const Bus& bus1 = view.getBus1(sw.getId());
    const Bus& bus2 = view.getBus2(sw.getId());
    context.getWriter().writeAttribute(BUS1, context.getAnonymizer().anonymizeString(bus1.getId()));
    context.getWriter().writeAttribute(BUS2, context.getAnonymizer().anonymizeString(bus2.getId()));
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

