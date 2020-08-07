/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlXmlSerializer.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlAdder.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

VoltagePerReactivePowerControlXmlSerializer::VoltagePerReactivePowerControlXmlSerializer() :
    AbstractExtensionXmlSerializer("voltagePerReactivePowerControl", "network", "vprpc", "http://www.powsybl.org/schema/iidm/ext/voltage_per_reactive_power_control/1_0") {
}

Extension& VoltagePerReactivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& slope = context.getReader().getAttributeValue<double>("slope");

    extendable.newExtension<VoltagePerReactivePowerControlAdder>().withSlope(slope).add();
    return extendable.getExtension<VoltagePerReactivePowerControl>();
}

void VoltagePerReactivePowerControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& ext = safeCast<VoltagePerReactivePowerControl>(extension);

    context.getExtensionsWriter().writeAttribute("slope", ext.getSlope());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
