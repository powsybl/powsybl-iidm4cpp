/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/StandbyAutomatonXmlSerializer.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/StandbyAutomaton.hpp>
#include <powsybl/iidm/extensions/iidm/StandbyAutomatonAdder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

StandbyAutomatonXmlSerializer::StandbyAutomatonXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("standbyAutomaton", "network", "sa", "http://www.itesla_project.eu/schema/iidm/ext/standby_automaton/1_0") {
}

Extension& StandbyAutomatonXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<StaticVarCompensator>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<StaticVarCompensator>()));
    }
    const auto& b0 = context.getReader().getAttributeValue<double>("b0");
    const auto& standby = context.getReader().getAttributeValue<bool>("standby");
    const auto& lowVoltageSetpoint = context.getReader().getAttributeValue<double>("lowVoltageSetPoint");
    const auto& highVoltageSetpoint = context.getReader().getAttributeValue<double>("highVoltageSetPoint");
    const auto& lowVoltageThreshold = context.getReader().getAttributeValue<double>("lowVoltageThreshold");
    const auto& highVoltageThreshold = context.getReader().getAttributeValue<double>("highVoltageThreshold");
    extendable.newExtension<StandbyAutomatonAdder>()
        .withB0(b0)
        .withStandby(standby)
        .withLowVoltageSetpoint(lowVoltageSetpoint)
        .withHighVoltageSetpoint(highVoltageSetpoint)
        .withLowVoltageThreshold(lowVoltageThreshold)
        .withHighVoltageThreshold(highVoltageThreshold)
        .add();
    return extendable.getExtension<StandbyAutomaton>();
}

void StandbyAutomatonXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& standbyAutomaton = safeCast<StandbyAutomaton>(extension);
    context.getWriter().writeAttribute("b0", standbyAutomaton.getB0());
    context.getWriter().writeAttribute("standby", standbyAutomaton.isStandby());
    context.getWriter().writeAttribute("lowVoltageSetPoint", standbyAutomaton.getLowVoltageSetpoint());
    context.getWriter().writeAttribute("highVoltageSetPoint", standbyAutomaton.getHighVoltageSetpoint());
    context.getWriter().writeAttribute("lowVoltageThreshold", standbyAutomaton.getLowVoltageThreshold());
    context.getWriter().writeAttribute("highVoltageThreshold", standbyAutomaton.getHighVoltageThreshold());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

