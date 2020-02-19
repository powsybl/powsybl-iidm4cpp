/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ShuntCompensatorXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ShuntCompensatorAdder ShuntCompensatorXml::createAdder(VoltageLevel& voltageLevel) const {
    return voltageLevel.newShuntCompensator();
}

const ShuntCompensatorXml& ShuntCompensatorXml::getInstance() {
    static ShuntCompensatorXml s_instance;
    return s_instance;
}

const char* ShuntCompensatorXml::getRootElementName() const {
    return SHUNT;
}

ShuntCompensator& ShuntCompensatorXml::readRootElementAttributes(ShuntCompensatorAdder& adder, NetworkXmlReaderContext& context) const {
    auto bPerSection = context.getReader().getAttributeValue<double>(B_PER_SECTION);
    auto maximumSectionCount = context.getReader().getAttributeValue<unsigned long>(MAXIMUM_SECTION_COUNT);
    auto currentSectionCount = context.getReader().getAttributeValue<unsigned long>(CURRENT_SECTION_COUNT);
    adder.setbPerSection(bPerSection)
        .setMaximumSectionCount(maximumSectionCount)
        .setCurrentSectionCount(currentSectionCount);
    readNodeOrBus(adder, context);
    ShuntCompensator& sc = adder.add();
    readPQ(sc.getTerminal(), context.getReader());
    return sc;
}

void ShuntCompensatorXml::readSubElements(ShuntCompensator& shuntCompensator, NetworkXmlReaderContext& context) const {
    context.getReader().readUntilEndElement(SHUNT, [this, &shuntCompensator, &context]() {
        AbstractIdentifiableXml::readSubElements(shuntCompensator, context);
    });
}

void ShuntCompensatorXml::writeRootElementAttributes(const ShuntCompensator& shuntCompensator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(B_PER_SECTION, shuntCompensator.getbPerSection());
    context.getWriter().writeAttribute(MAXIMUM_SECTION_COUNT, shuntCompensator.getMaximumSectionCount());
    context.getWriter().writeAttribute(CURRENT_SECTION_COUNT, shuntCompensator.getCurrentSectionCount());
    writeNodeOrBus(shuntCompensator.getTerminal(), context);
    writePQ(shuntCompensator.getTerminal(), context.getWriter());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

