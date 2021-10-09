/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControlXmlSerializer.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControlAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

HvdcAngleDroopActivePowerControlXmlSerializer::HvdcAngleDroopActivePowerControlXmlSerializer() :
    AbstractExtensionXmlSerializer("hvdcAngleDroopActivePowerControl", "network", "hapc", "http://www.itesla_project.eu/schema/iidm/ext/hvdc_angle_droop_active_power_control/1_0") {
}

Extension& HvdcAngleDroopActivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<HvdcLine>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<HvdcLine>()));
    }
    const double& p0 = context.getReader().getAttributeValue<double>("p0");
    const double& droop = context.getReader().getAttributeValue<double>("droop");
    const auto& enabled = context.getReader().getAttributeValue<bool>("enabled");
    extendable.newExtension<HvdcAngleDroopActivePowerControlAdder>().withP0(p0).withDroop(droop).withEnabled(enabled).add();
    return extendable.getExtension<HvdcAngleDroopActivePowerControl>();
}

void HvdcAngleDroopActivePowerControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& hadap = safeCast<HvdcAngleDroopActivePowerControl>(extension);
    context.getWriter().writeAttribute("p0", hadap.getP0());
    context.getWriter().writeAttribute("droop", hadap.getDroop());
    context.getWriter().writeAttribute("enabled", hadap.isEnabled());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
