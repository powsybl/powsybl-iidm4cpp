/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/NetworkXmlReaderContext.hpp>

#include <powsybl/iidm/converter/Anonymizer.hpp>
#include <powsybl/xml/XmlStreamReader.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

NetworkXmlReaderContext::NetworkXmlReaderContext(std::unique_ptr<Anonymizer>&& anonymizer, powsybl::xml::XmlStreamReader& reader, const ImportOptions& options, const IidmXmlVersion& version) :
    m_reader(reader),
    m_anonymizer(std::move(anonymizer)),
    m_options(options),
    m_version(version) {

}

void NetworkXmlReaderContext::addEndTask(const std::function<void()>& endTask) {
    m_endTasks.emplace_back(endTask);
}

void NetworkXmlReaderContext::buildExtensionNamespaceUriList(const stdcxx::const_range<ExtensionXmlSerializer>& providers) {
    for (const auto& it : providers) {
        std::string namespaceUri = m_reader.getNamespace(it.getNamespacePrefix(), "");
        if (!namespaceUri.empty()) {
            m_extensionsNamespaceUri.emplace(std::move(namespaceUri));
        }
    }
}

bool NetworkXmlReaderContext::containsExtensionNamespaceUri(const std::string& extensionNamespaceUri) const {
    return m_extensionsNamespaceUri.find(extensionNamespaceUri) != m_extensionsNamespaceUri.end();
}

const Anonymizer& NetworkXmlReaderContext::getAnonymizer() const {
    return *m_anonymizer;
}

const std::list<std::function<void()>>& NetworkXmlReaderContext::getEndTasks() const {
    return m_endTasks;
}

const ImportOptions& NetworkXmlReaderContext::getOptions() const {
    return m_options;
}

const powsybl::xml::XmlStreamReader& NetworkXmlReaderContext::getReader() const {
    return m_reader;
}

const IidmXmlVersion& NetworkXmlReaderContext::getVersion() const {
    return m_version;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
