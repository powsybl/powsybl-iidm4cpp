/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/BatteryShortCircuitXmlSerializer.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>
#include <powsybl/iidm/extensions/iidm/BatteryShortCircuit.hpp>
#include <powsybl/iidm/extensions/iidm/BatteryShortCircuitAdder.hpp>
#include <powsybl/iidm/extensions/iidm/Constants.hpp>
#include <powsybl/stdcxx/map.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BatteryShortCircuitXmlSerializer::BatteryShortCircuitXmlSerializer() :
    AbstractVersionableExtensionXmlSerializer("batteryShortCircuit", "network", "bsc",
        converter::xml::ExtensionXmlVersions({
            {"http://www.itesla_project.eu/schema/iidm/ext/battery_short_circuits/1_0", "bsc", "batteryShortCircuits",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_14(), {1,0}, "legacy"},
            {"http://www.itesla_project.eu/schema/iidm/ext/batteryshortcircuits/1_0", "bsc", "batteryShortCircuits",
            converter::xml::IidmXmlVersion::V1_0(), converter::xml::IidmXmlVersion::V1_14(), {1,0}, "legacy-2"},
            {"http://www.itesla_project.eu/schema/iidm/ext/battery_short_circuit/1_0", "bsc", "batteryShortCircuit",
            converter::xml::IidmXmlVersion::V1_13(), {1,0}}
        })){
}

const converter::xml::ExtensionXmlVersion& BatteryShortCircuitXmlSerializer::getVersion(const converter::xml::IidmXmlVersion& networkVersion) const {
    if (networkVersion < converter::xml::IidmXmlVersion::V1_13()) {
        return versionOf("1.0-legacy");
    }
    else if (networkVersion < converter::xml::IidmXmlVersion::V1_14()) {
        return versionOf("1.0");
    }
    return AbstractVersionableExtensionXmlSerializer::getVersion(networkVersion);
}

Extension& BatteryShortCircuitXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Battery>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
    }

    double transX = stdcxx::nan();
    double subtransX = stdcxx::nan();
    double stepUpTransformerX = stdcxx::nan();

    const auto& extensionVersion = getExtensionVersionImported(context);
    if(extensionVersion == versionOf("1.0-legacy") || extensionVersion == versionOf("1.0-legacy-2")) {
        transX = context.getReader().getAttributeValue<double>(TRANSIANT_REACTANCE);
        stepUpTransformerX = context.getReader().getOptionalAttributeValue(STEP_UP_TRANSFORMER_REACTANCE, stdcxx::nan());
    } else {
        subtransX = context.getReader().getOptionalAttributeValue(DIRECT_SUBTRANS_X, stdcxx::nan());
        transX = context.getReader().getAttributeValue<double>(DIRECT_TRANS_X);
        stepUpTransformerX = context.getReader().getOptionalAttributeValue(STEP_UP_TRANSFORMER_X, stdcxx::nan());
    }

    extendable.newExtension<BatteryShortCircuitAdder>()
                    .withDirectSubtransX(subtransX)
                    .withDirectTransX(transX)
                    .withStepUpTransformerX(stepUpTransformerX)
                    .add();
    return extendable.getExtension<BatteryShortCircuit>();
}

void BatteryShortCircuitXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& bsc = safeCast<BatteryShortCircuit>(extension);

    const auto& extensionVersion = getExtensionVersionToExport(context);
    if(extensionVersion == versionOf("1.0-legacy") || extensionVersion == versionOf("1.0-legacy-2")) {
        context.getWriter().writeAttribute(TRANSIANT_REACTANCE, bsc.getDirectTransX());
        context.getWriter().writeAttribute(STEP_UP_TRANSFORMER_REACTANCE, bsc.getStepUpTransformerX());
    } else {
        context.getWriter().writeAttribute(DIRECT_SUBTRANS_X, bsc.getDirectSubtransX());
        context.getWriter().writeAttribute(DIRECT_TRANS_X, bsc.getDirectTransX());
        context.getWriter().writeAttribute(STEP_UP_TRANSFORMER_X, bsc.getStepUpTransformerX());
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

