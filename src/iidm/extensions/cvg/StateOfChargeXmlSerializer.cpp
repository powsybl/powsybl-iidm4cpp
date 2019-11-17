/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/StateOfChargeXmlSerializer.hpp>

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/StateOfCharge.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

StateOfChargeXmlSerializer::StateOfChargeXmlSerializer() :
    ExtensionXmlSerializer("stateOfCharge", "network", "http://www.itesla_project.eu/schema/iidm/ext/state_of_charge/1_0", "soc") {
}

std::unique_ptr<Extension> StateOfChargeXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Battery>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
    }
    auto& battery = dynamic_cast<Battery&>(extendable);

    const auto& min = context.getReader().getAttributeValue<double>("min");
    const auto& max = context.getReader().getAttributeValue<double>("max");
    const auto& storageCapacity = context.getReader().getAttributeValue<double>("storageCapacity");
    const auto& current = context.getReader().getAttributeValue<double>("current");

    return stdcxx::make_unique<StateOfCharge>(battery, min, max, storageCapacity, current);
}

void StateOfChargeXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& stateOfCharge = safeCast<StateOfCharge>(extension);
    context.getExtensionsWriter().writeAttribute("min", stateOfCharge.getMin());
    context.getExtensionsWriter().writeAttribute("max", stateOfCharge.getMax());
    context.getExtensionsWriter().writeAttribute("storageCapacity", stateOfCharge.getStorageCapacity());
    context.getExtensionsWriter().writeAttribute("current", stateOfCharge.getCurrent());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
