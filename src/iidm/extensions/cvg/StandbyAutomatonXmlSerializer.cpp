/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/StandbyAutomatonXmlSerializer.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/StandbyAutomaton.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

StandbyAutomatonXmlSerializer::StandbyAutomatonXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("standbyAutomaton", "network", "http://www.itesla_project.eu/schema/iidm/ext/standby_automaton/1_0", "sa") {
}

std::unique_ptr<Extension> StandbyAutomatonXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<StaticVarCompensator>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<StaticVarCompensator>()));
    }
    auto& svc = dynamic_cast<StaticVarCompensator&>(extendable);

    const auto& b0 = context.getReader().getAttributeValue<double>("b0");
    const auto& standby = context.getReader().getAttributeValue<bool>("standby");
    const auto& lowVoltageSetpoint = context.getReader().getAttributeValue<double>("lowVoltageSetPoint");
    const auto& highVoltageSetpoint = context.getReader().getAttributeValue<double>("highVoltageSetPoint");
    const auto& lowVoltageThreshold = context.getReader().getAttributeValue<double>("lowVoltageThreshold");
    const auto& highVoltageThreshold = context.getReader().getAttributeValue<double>("highVoltageThreshold");
    return stdcxx::make_unique<StandbyAutomaton>(svc, b0, standby, lowVoltageSetpoint, highVoltageSetpoint, lowVoltageThreshold, highVoltageThreshold);
}

void StandbyAutomatonXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& standbyAutomaton = safeCast<StandbyAutomaton>(extension);
    context.getExtensionsWriter().writeAttribute("b0", standbyAutomaton.getB0());
    context.getExtensionsWriter().writeAttribute("standby", standbyAutomaton.isStandby());
    context.getExtensionsWriter().writeAttribute("lowVoltageSetPoint", standbyAutomaton.getLowVoltageSetpoint());
    context.getExtensionsWriter().writeAttribute("highVoltageSetPoint", standbyAutomaton.getHighVoltageSetpoint());
    context.getExtensionsWriter().writeAttribute("lowVoltageThreshold", standbyAutomaton.getLowVoltageThreshold());
    context.getExtensionsWriter().writeAttribute("highVoltageThreshold", standbyAutomaton.getHighVoltageThreshold());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

