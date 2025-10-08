/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "NetworkSourceXmlSerializer.hpp"

#include "NetworkSourceExt.hpp"
#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

NetworkSourceXmlSerializer::NetworkSourceXmlSerializer() :
    AbstractExtensionXmlSerializer("networkSource", "network", "extNetworkSource", "http://www.itesla_project.eu/schema/iidm/ext/networksource/1_0") {
}

Extension& NetworkSourceXmlSerializer::read(Extendable& extendable, NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Network>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Network>()));
    }
    auto& network = dynamic_cast<Network&>(extendable);

    const std::string& sourceData = context.getReader().getAttributeValue("sourceData");
    extendable.addExtension(stdcxx::make_unique<NetworkSourceExt>(network, sourceData));
    return extendable.getExtension<NetworkSourceExt>();
}

void NetworkSourceXmlSerializer::write(const Extension& extension, NetworkXmlWriterContext& context) const {
    const auto& networkSourceExt = safeCast<NetworkSourceExt>(extension);
    context.getWriter().writeAttribute("sourceData", networkSourceExt.getSourceData());
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
