/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HPP

#include <functional>
#include <string>
#include <vector>

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}

namespace iidm {

class PropertiesHolder;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class PropertiesXml {
public:
    static void read(PropertiesHolder& propertiesHolder,const NetworkXmlReaderContext& context);

    template <typename H, typename = typename std::enable_if<std::is_base_of<PropertiesHolder, H>::value>::type>
    static void read(std::vector<std::function<void(H&)>>& toApply, const NetworkXmlReaderContext& context);

    static void skip(const NetworkXmlReaderContext& context);

    static void write(const PropertiesHolder& propertiesHolder, const std::string& nsprefix, powsybl::xml::XmlStreamWriter& writer);
    static void write(const PropertiesHolder& propertiesHolder, NetworkXmlWriterContext& context);

private:
    template <typename H, typename = typename std::enable_if<std::is_base_of<PropertiesHolder, H>::value>::type>
    static std::function<void(H&)> read(const NetworkXmlReaderContext& context);

    PropertiesXml() = default;

    ~PropertiesXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/xml/PropertiesXml.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HPP
