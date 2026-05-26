/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ABSTRACTVERSIONABLEEXTENSIONXMLSERIALIZER_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ABSTRACTVERSIONABLEEXTENSIONXMLSERIALIZER_HPP

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlVersion.hpp>

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

    const std::string& getNamespacePrefix() const override;
    const std::string& getNamespacePrefix(const std::string& extensionVersion) const override;

    const std::string& getSerializationName(const std::string& extensionVersion) const override;

    std::set<std::string> getSerializationNames() const override;

    std::string getVersion() const override;

    /**
     * Get the greatest version of this extension serializer working with the given IIDM version
     * Throw an exception if none of the extension version supports it
     * Can be overwritten to define a different default version for a given IIDM version
     */
    virtual const ExtensionXmlVersion& getVersion(const IidmXmlVersion& networkVersion) const;

    /**
     * Checks if at least one extension version supports the given IIDM version
     */
    bool isIIDMVersionSupported(const IidmXmlVersion& networkVersion) const;

    std::set<std::string> getVersions() const override;

    void checkReadingCompatibility(const NetworkXmlReaderContext& networkContext) const override;

    void checkWritingCompatibility(const std::string& extensionVersion, const IidmXmlVersion& version) const;

private:
    /**
    * Throw exception if no extension version support the given IIDM version
    */
    void checkCompatibilityNetworkVersion(const IidmXmlVersion& version) const;

protected:
    virtual const ExtensionXmlVersion& getExtensionVersionImported(const NetworkXmlReaderContext& networkContext) const;
    virtual const ExtensionXmlVersion& getExtensionVersionToExport(const NetworkXmlWriterContext& networkContext) const;

    virtual const ExtensionXmlVersion& getDefaultVersion() const;

protected:
    /**
     * get the ExtensionXmlVersion of the given string representation, throws Exception if not found
     */
    const ExtensionXmlVersion& versionOf(const std::string& extensionVersion) const;
private:
    stdcxx::CReference<ExtensionXmlVersion> versionOf(const std::string& extensionVersion, bool throwIfUnknown) const;

public:

    AbstractVersionableExtensionXmlSerializer(std::string&& extensionName, std::string&& extensionCategory, std::string&& namespacePrefix,
                                             ExtensionXmlVersions&& versions);

    ~AbstractVersionableExtensionXmlSerializer() override = default;


private:
    ExtensionXmlVersions m_versions;

};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ABSTRACTVERSIONABLEEXTENSIONXMLSERIALIZER_HPP
