/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTVERSIONABLEEXTENSIONXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTVERSIONABLEEXTENSIONXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/VersionsCompatibity.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;

class AbstractVersionableExtensionXmlSerializer : public ExtensionXmlSerializer {
public:  // ExtensionXmlSerializer
    void checkExtensionVersionSupported(const std::string& extensionVersion) const override;

    const std::string& getNamespaceUri() const override;

    const std::string& getNamespaceUri(const std::string& extensionVersion) const override;

    const std::string& getVersion() const override;

    stdcxx::const_range<std::string> getVersions() const override;

public:
    AbstractVersionableExtensionXmlSerializer(std::string&& extensionName, std::string&& extensionCategory, std::string&& namespacePrefix,
                                              VersionsCompatibility&& extensionVersions, std::map<std::string, std::string>&& namespaceUris);

    ~AbstractVersionableExtensionXmlSerializer() override = default;

    void checkWritingCompatibility(const std::string& extensionVersion, const IidmXmlVersion& version) const;

    const std::string& getVersion(const IidmXmlVersion& networkVersion) const;

protected:
    void checkReadingCompatibility(const NetworkXmlReaderContext& networkContext) const;

private:
    VersionsCompatibility m_extensionVersions;

    std::map<std::string, std::string> m_namespaceUris;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTVERSIONABLEEXTENSIONXMLSERIALIZER_HPP
