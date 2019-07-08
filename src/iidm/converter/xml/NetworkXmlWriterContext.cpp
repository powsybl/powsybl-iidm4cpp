/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

NetworkXmlWriterContext::NetworkXmlWriterContext(Anonymizer& anonymizer, powsybl::xml::XmlStreamWriter& writer, const ExportOptions& options) :
    m_writer(writer),
    m_anonymizer(anonymizer),
    m_options(options) {

}

void NetworkXmlWriterContext::addExportedEquipment(const Identifiable& identifiable) {
    m_exportedEquipments.insert(identifiable.getId());
}

const Anonymizer& NetworkXmlWriterContext::getAnonymizer() const {
    return m_anonymizer;
}

Anonymizer& NetworkXmlWriterContext::getAnonymizer() {
    return m_anonymizer;
}

const std::set<std::string>& NetworkXmlWriterContext::getExportedEquipments() const {
    return m_exportedEquipments;
}

const ExportOptions& NetworkXmlWriterContext::getOptions() const {
    return m_options;
}

powsybl::xml::XmlStreamWriter& NetworkXmlWriterContext::getWriter() {
    return m_writer;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
