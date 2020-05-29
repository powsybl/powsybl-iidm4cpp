/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/stdcxx/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ExtensionXmlSerializer::ExtensionXmlSerializer(std::string&& extensionName, std::string&& categoryName, std::string&& namespacePrefix) :
    ExtensionProvider(std::move(extensionName), std::move(categoryName)),
    m_namespacePrefix(namespacePrefix) {
}

void ExtensionXmlSerializer::checkExtensionVersionSupported(const std::string& extensionVersion) const {
    if (extensionVersion != "1.0") {
        throw PowsyblException(stdcxx::format("The version %1% of the %2% extension's XML serializer is not supported", extensionVersion, getExtensionName()));
    }
}

const std::string& ExtensionXmlSerializer::getName() const {
    return getExtensionName();
}

const std::string& ExtensionXmlSerializer::getNamespacePrefix() const {
    return m_namespacePrefix;
}

const std::string& ExtensionXmlSerializer::getNamespaceUri() const {
    return getNamespaceUri(getVersion());
}

const std::string& ExtensionXmlSerializer::getVersion() const {
    static std::string s_version = "1.0";
    return s_version;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
