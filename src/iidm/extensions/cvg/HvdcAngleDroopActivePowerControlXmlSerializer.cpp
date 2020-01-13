/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/HvdcAngleDroopActivePowerControlXmlSerializer.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/HvdcAngleDroopActivePowerControl.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

HvdcAngleDroopActivePowerControlXmlSerializer::HvdcAngleDroopActivePowerControlXmlSerializer() :
    ExtensionXmlSerializer("hvdcAngleDroopActivePowerControl", "network", "http://www.itesla_project.eu/schema/iidm/ext/hvdc_angle_droop_active_power_control/1_0", "hapc") {
}

std::unique_ptr<Extension> HvdcAngleDroopActivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<HvdcLine>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<HvdcLine>()));
    }
    auto& hvdcLine = dynamic_cast<HvdcLine&>(extendable);

    const double& p0 = context.getReader().getAttributeValue<double>("p0");
    const double& droop = context.getReader().getAttributeValue<double>("droop");
    const auto& enabled = context.getReader().getAttributeValue<bool>("enabled");
    return stdcxx::make_unique<HvdcAngleDroopActivePowerControl>(hvdcLine, p0, droop, enabled);
}

void HvdcAngleDroopActivePowerControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& hadap = safeCast<HvdcAngleDroopActivePowerControl>(extension);
    context.getExtensionsWriter().writeAttribute("p0", hadap.getP0());
    context.getExtensionsWriter().writeAttribute("droop", hadap.getDroop());
    context.getExtensionsWriter().writeAttribute("enabled", hadap.isEnabled());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

