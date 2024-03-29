/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/BusbarSectionPositionXmlSerializer.hpp>

#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/sld/BusbarSectionPosition.hpp>
#include <powsybl/iidm/extensions/sld/BusbarSectionPositionAdder.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

BusbarSectionPositionXmlSerializer::BusbarSectionPositionXmlSerializer() :
    AbstractExtensionXmlSerializer("busbarSectionPosition", "network", "bbsp", "http://www.itesla_project.eu/schema/iidm/ext/busbarsectionposition/1_0") {
}

Extension& BusbarSectionPositionXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<BusbarSection>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<BusbarSection>()));
    }
    const auto& busbarIndex = context.getReader().getAttributeValue<unsigned long>("busbarIndex");
    const auto& sectionIndex = context.getReader().getAttributeValue<unsigned long>("sectionIndex");
    extendable.newExtension<BusbarSectionPositionAdder>().withBusbarIndex(busbarIndex).withSectionIndex(sectionIndex).add();
    return extendable.getExtension<BusbarSectionPosition>();
}

void BusbarSectionPositionXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& busbarSectionPosition = safeCast<BusbarSectionPosition>(extension);
    context.getWriter().writeAttribute("busbarIndex", busbarSectionPosition.getBusbarIndex());
    context.getWriter().writeAttribute("sectionIndex", busbarSectionPosition.getSectionIndex());
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
