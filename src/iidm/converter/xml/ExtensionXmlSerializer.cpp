/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

ExtensionXmlSerializer::ExtensionXmlSerializer(const std::string& extensionName, const std::string& categoryName,
                                               bool subElements, const std::string& namespaceUri, const std::string& namespacePrefix) :
    m_extensionName(extensionName),
    m_categoryName(categoryName),
    m_subElements(subElements),
    m_namespaceUri(namespaceUri),
    m_namespacePrefix(namespacePrefix) {
}

const std::string& ExtensionXmlSerializer::getCategoryName() const {
    return m_categoryName;
}

const std::string& ExtensionXmlSerializer::getExtensionName() const {
    return m_extensionName;
}

const std::string& ExtensionXmlSerializer::getNamespacePrefix() const {
    return m_namespacePrefix;
}

const std::string& ExtensionXmlSerializer::getNamespaceUri() const {
    return m_namespaceUri;
}

bool ExtensionXmlSerializer::hasSubElements() const {
    return m_subElements;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
