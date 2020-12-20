/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ShuntCompensatorXml.hpp"

#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/TerminalRefXml.hpp>

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
    IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &adder]() {
        bool voltageRegulatorOn = context.getReader().getOptionalAttributeValue(VOLTAGE_REGULATOR_ON, false);
        double targetV = context.getReader().getOptionalAttributeValue(TARGET_V, stdcxx::nan());
        double targetDeadband = context.getReader().getOptionalAttributeValue(TARGET_DEADBAND, stdcxx::nan());
        adder.setVoltageRegulatorOn(voltageRegulatorOn)
            .setTargetV(targetV)
            .setTargetDeadband(targetDeadband);
    });
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
        if (context.getReader().getLocalName() == REGULATING_TERMINAL) {
            IidmXmlUtil::assertMinimumVersion(SHUNT, REGULATING_TERMINAL, ErrorMessage::NOT_SUPPORTED, IidmXmlVersion::V1_2(), context.getVersion());
            const auto& id = context.getAnonymizer().deanonymizeString(context.getReader().getAttributeValue(ID));
            const auto& side = context.getReader().getOptionalAttributeValue(SIDE, "");
            context.addEndTask([id, side, &shuntCompensator]() {
                shuntCompensator.setRegulatingTerminal(stdcxx::ref<Terminal>(TerminalRefXml::readTerminalRef(shuntCompensator.getNetwork(), id, side)));
            });
        } else {
            AbstractIdentifiableXml::readSubElements(shuntCompensator, context);
        }
    });
}

void ShuntCompensatorXml::writeRootElementAttributes(const ShuntCompensator& shuntCompensator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    context.getWriter().writeAttribute(B_PER_SECTION, shuntCompensator.getbPerSection());
    context.getWriter().writeAttribute(MAXIMUM_SECTION_COUNT, shuntCompensator.getMaximumSectionCount());
    context.getWriter().writeAttribute(CURRENT_SECTION_COUNT, shuntCompensator.getCurrentSectionCount());
    IidmXmlUtil::writeBooleanAttributeFromMinimumVersion(SHUNT, VOLTAGE_REGULATOR_ON, shuntCompensator.isVoltageRegulatorOn(), false, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(SHUNT, TARGET_V, shuntCompensator.getTargetV(), ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    IidmXmlUtil::writeDoubleAttributeFromMinimumVersion(SHUNT, TARGET_DEADBAND, shuntCompensator.getTargetDeadband(), ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context);
    writeNodeOrBus(shuntCompensator.getTerminal(), context);
    writePQ(shuntCompensator.getTerminal(), context.getWriter());
}

void ShuntCompensatorXml::writeSubElements(const ShuntCompensator& shuntCompensator, const VoltageLevel& /*voltageLevel*/, NetworkXmlWriterContext& context) const {
    if (!stdcxx::areSame(shuntCompensator, shuntCompensator.getRegulatingTerminal().getConnectable().get())) {
        IidmXmlUtil::assertMinimumVersion(SHUNT, REGULATING_TERMINAL, ErrorMessage::NOT_DEFAULT_NOT_SUPPORTED, IidmXmlVersion::V1_2(), context.getVersion());
        IidmXmlUtil::runFromMinimumVersion(IidmXmlVersion::V1_2(), context.getVersion(), [&context, &shuntCompensator]() {
            TerminalRefXml::writeTerminalRef(shuntCompensator.getRegulatingTerminal(), context, REGULATING_TERMINAL);
        });
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

