/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/LoadDetailXmlSerializer.hpp>

#include <cmath>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/LoadDetail.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

LoadDetailXmlSerializer::LoadDetailXmlSerializer() :
    converter::xml::ExtensionXmlSerializer("detail", "network", "http://www.itesla_project.eu/schema/iidm/ext/load_detail/1_0", "ld") {
}

std::unique_ptr<Extension> LoadDetailXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    auto& load = dynamic_cast<Load&>(extendable);

    double fixedActivePower = context.getReader().getOptionalAttributeValue("subLoad1ActivePower", stdcxx::nan());
    if (std::isnan(fixedActivePower)) {
        fixedActivePower = context.getReader().getAttributeValue<double>("fixedActivePower");
    }
    double fixedReactivePower = context.getReader().getOptionalAttributeValue("subLoad1ReactivePower", stdcxx::nan());
    if (std::isnan(fixedReactivePower)) {
        fixedReactivePower = context.getReader().getAttributeValue<double>("fixedReactivePower");
    }
    double variableActivePower = context.getReader().getOptionalAttributeValue("subLoad2ActivePower", stdcxx::nan());
    if (std::isnan(variableActivePower)) {
        variableActivePower = context.getReader().getAttributeValue<double>("variableActivePower");
    }
    double variableReactivePower = context.getReader().getOptionalAttributeValue("subLoad2ReactivePower", stdcxx::nan());
    if (std::isnan(variableReactivePower)) {
        variableReactivePower = context.getReader().getAttributeValue<double>("variableReactivePower");
    }
    return stdcxx::make_unique<LoadDetail>(load, fixedActivePower, fixedReactivePower, variableActivePower, variableReactivePower);
}

void LoadDetailXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& detail = safeCast<LoadDetail>(extension);
    context.getExtensionsWriter().writeAttribute("fixedActivePower", detail.getFixedActivePower());
    context.getExtensionsWriter().writeAttribute("fixedReactivePower", detail.getFixedReactivePower());
    context.getExtensionsWriter().writeAttribute("variableActivePower", detail.getVariableActivePower());
    context.getExtensionsWriter().writeAttribute("variableReactivePower", detail.getVariableReactivePower());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

