/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/CongestionManagementXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/CongestionManagement.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

CongestionManagementXmlSerializer::CongestionManagementXmlSerializer() :
    ExtensionXmlSerializer("congestionManagement", "network", "http://www.itesla_project.eu/schema/iidm/ext/congestion_management/1_0", "cm") {
}

std::unique_ptr<Extension> CongestionManagementXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Battery>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
    }
    auto& battery = dynamic_cast<Battery&>(extendable);

    const auto& congestionManagementOn = context.getReader().getAttributeValue<bool>("enabled");
    return stdcxx::make_unique<CongestionManagement>(battery, congestionManagementOn);
}

void CongestionManagementXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& congestionManagement = safeCast<CongestionManagement>(extension);
    context.getExtensionsWriter().writeAttribute("enabled", congestionManagement.isEnabled());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

