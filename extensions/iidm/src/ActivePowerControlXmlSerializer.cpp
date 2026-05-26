/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLXMLSERIALIZER_HXX
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLXMLSERIALIZER_HXX

#include <powsybl/iidm/extensions/iidm/ActivePowerControlXmlSerializer.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Generator.hpp>

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>

#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControlAdder.hpp>

#include <powsybl/stdcxx/map.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ActivePowerControlXmlSerializer::ActivePowerControlXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("activePowerControl", "network", "apc",
        converter::xml::ExtensionXmlVersions({
            {"http://www.itesla_project.eu/schema/iidm/ext/active_power_control/1_0", "apc", "activePowerControl",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_13(), {1,0}},
            {"http://www.powsybl.org/schema/iidm/ext/active_power_control/1_1", "apc", "activePowerControl",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_13(), {1,1}},
            {"http://www.powsybl.org/schema/iidm/ext/active_power_control/1_2", "apc", "activePowerControl",
            converter::xml::IidmXmlVersion::V1_13(), {1,2}}
        })){
}

Extension& ActivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& participate = context.getReader().getAttributeValue<bool>("participate");
    const auto& droop = context.getReader().getAttributeValue<double>("droop");
    double participationFactor = 0.0;
    double minTargetP = stdcxx::nan();
    double maxTargetP = stdcxx::nan();
    
    const auto& extensionVersion = getExtensionVersionImported(context);
    if (extensionVersion >= versionOf("1.1")) {
        participationFactor = context.getReader().getOptionalAttributeValue("participationFactor", 0.0);
    }
    if (extensionVersion >= versionOf("1.2")) {
        maxTargetP = context.getReader().getOptionalAttributeValue("maxTargetP", stdcxx::nan());
        minTargetP = context.getReader().getOptionalAttributeValue("minTargetP", stdcxx::nan());
    }

    extendable.newExtension<ActivePowerControlAdder>()
        .withParticipate(participate)
        .withDroop(droop)
        .withParticipationFactor(participationFactor)
        .withMinTargetP(minTargetP)
        .withMaxTargetP(maxTargetP)
        .add();
    return extendable.getExtension<ActivePowerControl>();
}

void ActivePowerControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& apc = safeCast<ActivePowerControl>(extension);

    context.getWriter().writeAttribute("participate", apc.isParticipate());
    context.getWriter().writeAttribute("droop", apc.getDroop());

    const auto& extensionVersion = getExtensionVersionToExport(context);
    if (extensionVersion >= versionOf("1.1")) {
        context.getWriter().writeAttribute("participationFactor", apc.getParticipationFactor());
    }
    if (extensionVersion >= versionOf("1.2")) {
        context.getWriter().writeAttribute("maxTargetP", apc.getMaxTargetP());
        context.getWriter().writeAttribute("minTargetP", apc.getMinTargetP());
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLXMLSERIALIZER_HXX
