/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadDetailXmlSerializer.hpp"

#include <cmath>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/LoadDetail.hpp>
#include <powsybl/iidm/extensions/LoadDetailAdder.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

LoadDetailXmlSerializer::LoadDetailXmlSerializer() :
    converter::xml::AbstractExtensionXmlSerializer("detail", "network", "ld", "http://www.itesla_project.eu/schema/iidm/ext/load_detail/1_0") {
}

Extension& LoadDetailXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<Load>(extendable)) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Load>()));
    }
    double fixedActivePower = context.getReader().getOptionalAttributeValue("fixedActivePower", stdcxx::nan());
    if (std::isnan(fixedActivePower)) {
        fixedActivePower = context.getReader().getAttributeValue<double>("subLoad1ActivePower");
    }
    double fixedReactivePower = context.getReader().getOptionalAttributeValue("fixedReactivePower", stdcxx::nan());
    if (std::isnan(fixedReactivePower)) {
        fixedReactivePower = context.getReader().getAttributeValue<double>("subLoad1ReactivePower");
    }
    double variableActivePower = context.getReader().getOptionalAttributeValue("variableActivePower", stdcxx::nan());
    if (std::isnan(variableActivePower)) {
        variableActivePower = context.getReader().getAttributeValue<double>("subLoad2ActivePower");
    }
    double variableReactivePower = context.getReader().getOptionalAttributeValue("variableReactivePower", stdcxx::nan());
    if (std::isnan(variableReactivePower)) {
        variableReactivePower = context.getReader().getAttributeValue<double>("subLoad2ReactivePower");
    }
    extendable.newExtension<iidm::extensions::LoadDetailAdder>()
            .withFixedActivePower(fixedActivePower)
            .withFixedReactivePower(fixedReactivePower)
            .withVariableActivePower(variableActivePower)
            .withVariableReactivePower(variableReactivePower)
            .add();
    return extendable.getExtension<iidm::extensions::LoadDetail>();
}

void LoadDetailXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& detail = safeCast<iidm::extensions::LoadDetail>(extension);
    context.getWriter().writeAttribute("fixedActivePower", detail.getFixedActivePower());
    context.getWriter().writeAttribute("fixedReactivePower", detail.getFixedReactivePower());
    context.getWriter().writeAttribute("variableActivePower", detail.getVariableActivePower());
    context.getWriter().writeAttribute("variableReactivePower", detail.getVariableReactivePower());
}

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
