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
#include <powsybl/iidm/extensions/iidm/CoordinatedReactiveControlAdder.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

CoordinatedReactiveControlXmlSerializer::CoordinatedReactiveControlXmlSerializer() :
    AbstractExtensionXmlSerializer("coordinatedReactiveControl", "network", "crc", "http://www.powsybl.org/schema/iidm/ext/coordinated_reactive_control/1_0") {
}

Extension& CoordinatedReactiveControlXmlSerializer::read(Extendable& extendable, converter::xml::NetworkXmlReaderContext& context) const {
    const auto& qPercent = context.getReader().getAttributeValue<double>("qPercent");

    extendable.newExtension<CoordinatedReactiveControlAdder>().withQPercent(qPercent).add();
    return extendable.getExtension<CoordinatedReactiveControl>();
}

void CoordinatedReactiveControlXmlSerializer::write(const Extension& extension, converter::xml::NetworkXmlWriterContext& context) const {
    const auto& crc = safeCast<CoordinatedReactiveControl>(extension);
    context.getWriter().writeAttribute("qPercent", crc.getQPercent());
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

