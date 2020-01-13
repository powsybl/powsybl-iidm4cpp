/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/BusbarSectionPositionXmlSerializer.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/BusbarSectionPosition.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

BusbarSectionPositionXmlSerializer::BusbarSectionPositionXmlSerializer() :
    ExtensionXmlSerializer("busbarSectionPosition", "network", "http://www.itesla_project.eu/schema/iidm/ext/busbarsectionposition/1_0", "bbsp") {
}

std::unique_ptr<Extension> BusbarSectionPositionXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<BusbarSection>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<BusbarSection>()));
    }
    auto& busbarSection = dynamic_cast<BusbarSection&>(extendable);

    const auto& busbarIndex = context.getReader().getAttributeValue<unsigned long>("busbarIndex");
    const auto& sectionIndex = context.getReader().getAttributeValue<unsigned long>("sectionIndex");
    return stdcxx::make_unique<BusbarSectionPosition>(busbarSection, busbarIndex, sectionIndex);
}

void BusbarSectionPositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& busbarSectionPosition = safeCast<BusbarSectionPosition>(extension);
    context.getExtensionsWriter().writeAttribute("busbarIndex", busbarSectionPosition.getBusbarIndex());
    context.getExtensionsWriter().writeAttribute("sectionIndex", busbarSectionPosition.getSectionIndex());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

