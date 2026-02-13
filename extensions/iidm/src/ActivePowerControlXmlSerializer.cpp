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
        converter::xml::VersionsCompatibilityBuilder()
            .put(converter::xml::IidmXmlVersion::V1_0(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_1(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_2(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_3(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_4(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_5(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_6(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_7(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_8(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_9(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_10(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_11(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_12(), {"1.0", "1.1"})
            .put(converter::xml::IidmXmlVersion::V1_13(), {"1.2"})
            .put(converter::xml::IidmXmlVersion::V1_14(), {"1.2"})
            .build(),
        stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0", "http://www.itesla_project.eu/schema/iidm/ext/active_power_control/1_0")
            .put("1.1", "http://www.powsybl.org/schema/iidm/ext/active_power_control/1_1")
            .put("1.2", "http://www.powsybl.org/schema/iidm/ext/active_power_control/1_2")
            .build()) {
}

Extension& ActivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& participate = context.getReader().getAttributeValue<bool>("participate");
    const auto& droop = context.getReader().getAttributeValue<double>("droop");
    double participationFactor = 0.0;
    double minTargetP = stdcxx::nan();
    double maxTargetP = stdcxx::nan();
    
    const std::string& extensionVersionStr = context.getExtensionVersion(*this);
    if (extensionVersionStr.empty()) {
        throw AssertionError("Extension version not found");
    }
    if(extensionVersionStr.compare("1.1") >= 0) {
        participationFactor = context.getReader().getOptionalAttributeValue("participationFactor", 0.0);
    }
    if (extensionVersionStr.compare("1.2") >= 0) {
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

    std::string extVersionStr = context.getExtensionVersion("activePowerControl");
    if (extVersionStr.empty()) {
        extVersionStr = getVersion(context.getVersion());
    }
    if(extVersionStr.compare("1.1") >= 0) {
        context.getWriter().writeAttribute("participationFactor", apc.getParticipationFactor());
    }
    if (extVersionStr.compare("1.2") >= 0) {
        context.getWriter().writeAttribute("maxTargetP", apc.getMaxTargetP());
        context.getWriter().writeAttribute("minTargetP", apc.getMinTargetP());
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLXMLSERIALIZER_HXX
