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

NetworkXmlWriterContext::NetworkXmlWriterContext(std::unique_ptr<Anonymizer>&& anonymizer, powsybl::xml::XmlStreamWriter& writer, const ExportOptions& options, const BusFilter& filter, const IidmXmlVersion& version) :
    m_writer(writer),
    m_anonymizer(std::move(anonymizer)),
    m_options(options),
    m_version(version),
    m_filter(filter) {

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

powsybl::xml::XmlStreamWriter& NetworkXmlWriterContext::getWriter() {
    return m_writer;
}

bool NetworkXmlWriterContext::isExportedEquipment(const std::string& id) {
    return m_exportedEquipments.find(id) != m_exportedEquipments.end();
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
