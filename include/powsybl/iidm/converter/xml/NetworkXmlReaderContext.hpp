/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLREADERCONTEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLREADERCONTEXT_HPP

#include <functional>
#include <list>
#include <memory>

#include <powsybl/iidm/converter/ImportOptions.hpp>
#include <powsybl/iidm/converter/xml/ExtensionXmlSerializer.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>
#include <powsybl/iidm/converter/xml/XmlReaderEndTask.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace xml {

class XmlStreamReader;

}  // namespace xml

namespace iidm {

class Validable;

namespace converter {

class Anonymizer;

namespace xml {

class NetworkXmlReaderContext {
public:
    NetworkXmlReaderContext(std::unique_ptr<Anonymizer>&& anonymizer, powsybl::xml::XmlStreamReader& reader, const ImportOptions& options, const IidmXmlVersion& version);

    void addEndTask(const XmlReaderEndTask::Step& step, const std::function<void()>& endTask);

    void checkAndAddExtensionNamespaceUri();

    bool containsExtensionNamespaceUri(const std::string& extensionNamespaceUri) const;

    const Anonymizer& getAnonymizer() const;

    /**
     * Returns all EndTasks
     */
    const std::map<XmlReaderEndTask::Step, std::list<XmlReaderEndTask>>& getEndTasks() const;

    /**
     * Executes all tasks associated to the given Step and all previous tasks that are still not processed yet
     */
    void executeEndTasks(const XmlReaderEndTask::Step& step);

    const std::string& getExtensionVersion(const ExtensionXmlSerializer& extensionXmlSerializer) const;

    const ImportOptions& getOptions() const;

    const powsybl::xml::XmlStreamReader& getReader() const;

    const IidmXmlVersion& getVersion() const;

    NetworkXmlReaderContext& setNetworkValidationLevel(const ValidationLevel& networkValidationLevel);

    const ValidationLevel& getNetworkValidationLevel() const;

    void addIgnoredEquipment(const std::string& equipmentId);

    bool isIgnoredEquipment(const std::string& equipmentId) const;

private:
    powsybl::xml::XmlStreamReader& m_reader;

    std::unique_ptr<Anonymizer> m_anonymizer;

    std::map<XmlReaderEndTask::Step, std::list<XmlReaderEndTask>> m_endTasks;

    ImportOptions m_options;

    const IidmXmlVersion& m_version;

    std::set<std::string> m_extensionsNamespaceUri;

    ValidationLevel m_networkValidationLevel;

    std::set<std::string> m_ignoredEquipments;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLREADERCONTEXT_HPP
