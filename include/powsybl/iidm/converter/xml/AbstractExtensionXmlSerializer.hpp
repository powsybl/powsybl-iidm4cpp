/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTEXTENSIONXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTEXTENSIONXMLSERIALIZER_HPP

#include <string>

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

class Extension;
class Identifiable;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class AbstractExtensionXmlSerializer : public ExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    const std::string& getNamespaceUri() const override;

    const std::string& getNamespaceUri(const std::string& extensionVersion) const override;

public:
    AbstractExtensionXmlSerializer(std::string&& extensionName, std::string&& categoryName, std::string&& namespacePrefix, std::string&& namespaceUri);

    ~AbstractExtensionXmlSerializer() noexcept override = default;

private:
    std::string m_namespaceUri;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTEXTENSIONXMLSERIALIZER_HPP
