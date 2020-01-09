/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/HvdcOperatorActivePowerRangeXmlSerializer.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/iidm/extensions/cvg/HvdcOperatorActivePowerRange.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

HvdcOperatorActivePowerRangeXmlSerializer::HvdcOperatorActivePowerRangeXmlSerializer() :
    ExtensionXmlSerializer("hvdcOperatorActivePowerRange", "network", "http://www.itesla_project.eu/schema/iidm/ext/hvdc_operator_active_power_range/1_0", "hopr") {
}

std::unique_ptr<Extension> HvdcOperatorActivePowerRangeXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    if (!stdcxx::isInstanceOf<HvdcLine>(extendable)) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<HvdcLine>()));
    }
    auto& hvdcLine = dynamic_cast<HvdcLine&>(extendable);

    const double& oprFromCS1toCS2 = context.getReader().getOptionalAttributeValue("fromCS1toCS2", stdcxx::nan());
    const double& oprFromCS2toCS1 = context.getReader().getOptionalAttributeValue("fromCS2toCS1", stdcxx::nan());
    return stdcxx::make_unique<HvdcOperatorActivePowerRange>(hvdcLine, oprFromCS1toCS2, oprFromCS2toCS1);
}

void HvdcOperatorActivePowerRangeXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& opr = safeCast<HvdcOperatorActivePowerRange>(extension);
    context.getExtensionsWriter().writeOptionalAttribute("fromCS1toCS2", opr.getOprFromCS1toCS2());
    context.getExtensionsWriter().writeOptionalAttribute("fromCS2toCS1", opr.getOprFromCS2toCS1());
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

