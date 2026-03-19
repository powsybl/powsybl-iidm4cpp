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
    m_version(version),
    m_networkValidationLevel(ValidationLevel::STEADY_STATE_HYPOTHESIS) {

}

void NetworkXmlReaderContext::addEndTask(const XmlReaderEndTask::Step& step, const std::function<void()>& endTask) {
    m_endTasks.emplace_back(XmlReaderEndTask(step, endTask));
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

const std::list<XmlReaderEndTask>& NetworkXmlReaderContext::getEndTasks() const {
    return m_endTasks;
}

void NetworkXmlReaderContext::executeEndTasks(const XmlReaderEndTask::Step& step) {

    //previous steps that might not have been run yet
    for (auto& endTask : m_endTasks) {
        if(endTask.getStep() < step && !endTask.isProcessed()) {
            endTask.runTask();
        }
    }

    //current step
    for (auto& endTask : m_endTasks) {
        if(endTask.getStep() == step) {
            endTask.runTask();
        }
    }
}

const std::string& NetworkXmlReaderContext::getExtensionVersion(const ExtensionXmlSerializer& extensionXmlSerializer) const {
    static std::string s_version;
    for (const std::string& version : extensionXmlSerializer.getVersions()) {
        auto it = m_extensionsNamespaceUri.find(extensionXmlSerializer.getNamespaceUri(version));
        if (it != m_extensionsNamespaceUri.end()) {
            return version;
        }
    }
    return s_version;
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

NetworkXmlReaderContext& NetworkXmlReaderContext::setNetworkValidationLevel(const ValidationLevel& networkValidationLevel) {
    m_networkValidationLevel = networkValidationLevel;
    return *this;
}

const ValidationLevel& NetworkXmlReaderContext::getNetworkValidationLevel() const {
    return m_networkValidationLevel;
}


void NetworkXmlReaderContext::addIgnoredEquipment(const std::string& equipmentId) {
    m_ignoredEquipments.insert(equipmentId);
}

bool NetworkXmlReaderContext::isIgnoredEquipment(const std::string& equipmentId) const {
    return m_ignoredEquipments.find(equipmentId)!=m_ignoredEquipments.cend();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
