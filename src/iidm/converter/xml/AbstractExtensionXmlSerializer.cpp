/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/AbstractExtensionXmlSerializer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

AbstractExtensionXmlSerializer::AbstractExtensionXmlSerializer(std::string extensionName, std::string categoryName, std::string namespacePrefix, std::string namespaceUri) :
    ExtensionXmlSerializer(std::move(extensionName), std::move(categoryName), std::move(namespacePrefix)),
    m_namespaceUri(std::move(namespaceUri)) {
}

const std::string& AbstractExtensionXmlSerializer::getNamespaceUri() const {
    return m_namespaceUri;
}

const std::string& AbstractExtensionXmlSerializer::getNamespaceUri(const std::string& /*extensionVersion*/) const {
    return m_namespaceUri;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
