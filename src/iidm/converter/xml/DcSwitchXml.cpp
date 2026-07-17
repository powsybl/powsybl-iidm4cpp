/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "DcSwitchXml.hpp"

#include <powsybl/iidm/DcNode.hpp>
#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

DcSwitchAdder DcSwitchXml::createAdder(Network& network) const {
    return network.newDcSwitch();
}

const DcSwitchXml& DcSwitchXml::getInstance() {
    static DcSwitchXml s_instance;
    return s_instance;
}

const char* DcSwitchXml::getRootElementName() const {
    return DC_SWITCH;
}

DcSwitch& DcSwitchXml::readRootElementAttributes(DcSwitchAdder& adder, Network& /*network*/, NetworkXmlReaderContext& context) const {
    const auto& dcNode1Id = context.getReader().getAttributeValue(toString(DC_NODE, 1));
    const auto& dcNode2Id = context.getReader().getAttributeValue(toString(DC_NODE, 2));

    const auto& switchKind = Enum::fromString<DcSwitchKind>(context.getReader().getAttributeValue(KIND));
    const auto& open = context.getReader().getAttributeValue<bool>(OPEN);

    return adder.setDcNode1(dcNode1Id)
                            .setDcNode2(dcNode2Id)
                            .setKind(switchKind)
                            .setOpen(open)
                            .add();

}

void DcSwitchXml::readSubElements(DcSwitch& dcSwitch, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(DC_SWITCH, [this, &dcSwitch, &context]() {
        AbstractSimpleIdentifiableXml::readSubElements(dcSwitch, context);
    });
}

void DcSwitchXml::writeRootElementAttributes(const DcSwitch& dcSwitch, const Network& /*network*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(toString(DC_NODE, 1), dcSwitch.getDcNode1().getId());
    context.getWriter().writeAttribute(toString(DC_NODE, 2), dcSwitch.getDcNode2().getId());

    context.getWriter().writeAttribute(KIND, Enum::toString(dcSwitch.getKind()));
    context.getWriter().writeAttribute(OPEN, dcSwitch.isOpen());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

