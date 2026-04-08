/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HXX
#define POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HXX

#include <powsybl/iidm/converter/xml/PropertiesXml.hpp>

#include <powsybl/iidm/PropertiesHolder.hpp>
#include <powsybl/iidm/converter/Constants.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

template <typename H, typename>
void PropertiesXml::read(std::vector<std::function<void(H&)>>& toApply, const NetworkXmlReaderContext& context) {
    toApply.emplace_back(read<H>(context));
}

template <typename H, typename>
std::function<void(H&)> PropertiesXml::read(const NetworkXmlReaderContext& context) {
    if(context.getReader().getLocalName() != PROPERTY) {
        throw PowsyblException(stdcxx::format("Unexpected element name <%1%> (expected <%2%>)", context.getReader().getLocalName(), PROPERTY));
    }

    const std::string& name = context.getReader().getAttributeValue(NAME);
    const std::string& value = context.getReader().getAttributeValue(VALUE);

    std::function<void(H&)> fun = [name,value](H &propertiesHolder) {
        propertiesHolder.setProperty(name, value);
    };

    return fun;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HXX