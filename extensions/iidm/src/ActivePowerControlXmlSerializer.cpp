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

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ActivePowerControlXmlSerializer::ActivePowerControlXmlSerializer() :
    AbstractExtensionXmlSerializer("activePowerControl", "network", "apc", "http://www.itesla_project.eu/schema/iidm/ext/active_power_control/1_0") {
}

std::unique_ptr<Extension> ActivePowerControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& participate = context.getReader().getAttributeValue<bool>("participate");
    const auto& droop = context.getReader().getAttributeValue<double>("droop");

    if (stdcxx::isInstanceOf<Battery>(extendable)) {
        auto& battery = dynamic_cast<Battery&>(extendable);
        return stdcxx::make_unique<Extension, ActivePowerControl>(battery, participate, droop);
    }
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        auto& generator = dynamic_cast<Generator&>(extendable);
        return stdcxx::make_unique<Extension, ActivePowerControl>(generator, participate, droop);
    }

    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

void ActivePowerControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& apc = safeCast<ActivePowerControl>(extension);

    context.getExtensionsWriter().writeAttribute("participate", apc.isParticipate());
    context.getExtensionsWriter().writeAttribute("droop", apc.getDroop());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ACTIVEPOWERCONTROLXMLSERIALIZER_HXX
