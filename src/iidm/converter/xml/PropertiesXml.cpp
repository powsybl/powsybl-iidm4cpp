/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>

#include <powsybl/iidm/PropertiesHolder.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>
#include <powsybl/xml/XmlStreamWriter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

void PropertiesXml::read(PropertiesHolder& propertiesHolder, const NetworkXmlReaderContext& context) {
    read<PropertiesHolder>(context)(propertiesHolder);
}

void PropertiesXml::write(const PropertiesHolder& propertiesHolder, NetworkXmlWriterContext& context) {
    for (const auto& name : propertiesHolder.getPropertyNames()) {
        const auto& value = propertiesHolder.getProperty(name);
        context.getWriter().writeStartElement(context.getVersion().getPrefix(), PROPERTY);
        context.getWriter().writeAttribute(NAME, name);
        context.getWriter().writeAttribute(VALUE, value);
        context.getWriter().writeEndElement();
    }
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
