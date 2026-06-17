/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/converter/Anonymizer.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

NetworkXmlWriterContext::NetworkXmlWriterContext(std::unique_ptr<Anonymizer>&& anonymizer, powsybl::xml::XmlStreamWriter& writer, const ExportOptions& options, const BusFilter& filter, const IidmXmlVersion& version, bool valid) :
    m_writer(writer),
    m_anonymizer(std::move(anonymizer)),
    m_options(options),
    m_version(version),
    m_filter(filter),
    m_valid(valid) {

}

void NetworkXmlWriterContext::addExportedEquipment(const Identifiable& identifiable) {
    m_exportedEquipments.insert(identifiable.getId());
}

const Anonymizer& NetworkXmlWriterContext::getAnonymizer() const {
    return *m_anonymizer;
}

Anonymizer& NetworkXmlWriterContext::getAnonymizer() {
    return *m_anonymizer;
}

const std::set<std::string>& NetworkXmlWriterContext::getExportedEquipments() const {
    return m_exportedEquipments;
}

const std::string& NetworkXmlWriterContext::getExtensionVersion(const std::string& extensionName) const {
    return m_options.getExtensionVersion(extensionName);
}

const BusFilter& NetworkXmlWriterContext::getFilter() const {
    return m_filter;
}

const ExportOptions& NetworkXmlWriterContext::getOptions() const {
    return m_options;
}

const IidmXmlVersion& NetworkXmlWriterContext::getVersion() const {
    return m_version;
}

bool NetworkXmlWriterContext::isValid() const {
    return m_valid;
}

powsybl::xml::XmlStreamWriter& NetworkXmlWriterContext::getWriter() {
    return m_writer;
}

bool NetworkXmlWriterContext::isExportedEquipment(const std::string& id) {
    return m_exportedEquipments.find(id) != m_exportedEquipments.end();
}

void NetworkXmlWriterContext::addExtensionFixedPrefix(const std::string& extensionName, const std::string& fixedPrefix) {
    m_fixedExtensionPrefixes[extensionName] = fixedPrefix;
}

std::string NetworkXmlWriterContext::getExtensionFixedPrefix(const std::string& extensionName) const {
    const auto& it = m_fixedExtensionPrefixes.find(extensionName);
    if(it != m_fixedExtensionPrefixes.end()) {
        return it->second;
    }
    return "";
}

void NetworkXmlWriterContext::addVoltageLevelExportTopologyLevel(const std::string& voltageLevelId, const TopologyLevel& topologyLevel) {
    m_voltageLevelExportTopologyLevels.insert(std::make_pair(voltageLevelId, topologyLevel));
}

stdcxx::optional<TopologyLevel> NetworkXmlWriterContext::getVoltageLevelTopologyLevel(const std::string& voltageLevelId) const {
    if(m_voltageLevelExportTopologyLevels.count(voltageLevelId)) {
        return m_voltageLevelExportTopologyLevels.at(voltageLevelId);
    }
    return stdcxx::optional<TopologyLevel>();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
