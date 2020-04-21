/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlXmlSerializer.hpp>

#include <powsybl/AssertionError.hpp>

#include <powsybl/iidm/Generator.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControl.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

CoordinatedReactiveControlXmlSerializer::CoordinatedReactiveControlXmlSerializer() :
    ExtensionXmlSerializer("coordinatedReactiveControl", "network", "http://www.itesla_project.eu/schema/iidm/ext/coordinated_reactive_control/1_0", "crc") {
}

std::unique_ptr<Extension> CoordinatedReactiveControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Generator>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
    }
    auto& generator = dynamic_cast<Generator&>(extendable);

    const auto& qPercent = context.getReader().getAttributeValue<double>("qPercent");

    return stdcxx::make_unique<Extension, CoordinatedReactiveControl>(generator, qPercent);
}

void CoordinatedReactiveControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& crc = safeCast<CoordinatedReactiveControl>(extension);
    context.getExtensionsWriter().writeAttribute("qPercent", crc.getQPercent());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

