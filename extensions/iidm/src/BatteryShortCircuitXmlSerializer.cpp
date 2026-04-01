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
    converter::xml::AbstractVersionableExtensionXmlSerializer("batteryShortCircuit", "network", "bsc",
        converter::xml::VersionsCompatibilityBuilder()
            .put(converter::xml::IidmXmlVersion::V1_0(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_1(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_2(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_3(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_4(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_5(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_6(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_7(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_8(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_9(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_10(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_11(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_12(), {"1.0-legacy-2", "1.0-legacy"})
            .put(converter::xml::IidmXmlVersion::V1_13(), {"1.0-legacy-2", "1.0-legacy", "1.0"})
            .put(converter::xml::IidmXmlVersion::V1_14(), {"1.0"})
            .build(),
        stdcxx::MapBuilder<std::string, std::string>()
            .put("1.0-legacy-2", "http://www.itesla_project.eu/schema/iidm/ext/batteryshortcircuits/1_0")
            .put("1.0-legacy", "http://www.itesla_project.eu/schema/iidm/ext/battery_short_circuits/1_0")
            .put("1.0", "http://www.itesla_project.eu/schema/iidm/ext/battery_short_circuit/1_0")
            .build(),
        stdcxx::MapBuilder<std::string, AlternativeSerializationData>()
            .put("batteryShortCircuits", {{"1.0-legacy-2", "1.0-legacy"}, ""})
            .build()) {
}

Extension& BatteryShortCircuitXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Battery>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
    }

    double transX = stdcxx::nan();
    double subtransX = stdcxx::nan();
    double stepUpTransformerX = stdcxx::nan();

    const std::string& extensionVersionStr = context.getExtensionVersion(*this);
    if (extensionVersionStr.empty()) {
        throw AssertionError("Extension version not found");
    }
    if(extensionVersionStr.compare("1.0-legacy") == 0 || extensionVersionStr.compare("1.0-legacy-2") == 0) {
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

    std::string extVersionStr = context.getExtensionVersion(getName());
    if (extVersionStr.empty()) {
        extVersionStr = getVersion(context.getVersion());
    }
    if(extVersionStr.compare("1.0-legacy") == 0 || extVersionStr.compare("1.0-legacy-2") == 0) {
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

