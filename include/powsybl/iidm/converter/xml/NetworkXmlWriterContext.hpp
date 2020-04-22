/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP

#include <functional>
#include <set>
#include <string>

#include <powsybl/iidm/converter/BusFilter.hpp>
#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

namespace powsybl {

namespace xml {

class XmlStreamWriter;

}  // namespace xml

namespace iidm {

class Identifiable;

namespace converter {

class Anonymizer;

namespace xml {

class NetworkXmlWriterContext {
public:
    NetworkXmlWriterContext(Anonymizer& anonymizer, powsybl::xml::XmlStreamWriter& writer, const ExportOptions& options, const BusFilter& filter, const IidmXmlVersion& version);

    ~NetworkXmlWriterContext() = default;

    void addExportedEquipment(const Identifiable& identifiable);

    const Anonymizer& getAnonymizer() const;

    Anonymizer& getAnonymizer();

    const std::set<std::string>& getExportedEquipments() const;

    powsybl::xml::XmlStreamWriter& getExtensionsWriter();

    const BusFilter& getFilter() const;

    const ExportOptions& getOptions() const;

    const IidmXmlVersion& getVersion() const;

    powsybl::xml::XmlStreamWriter& getWriter();

    bool isExportedEquipment(const std::string& id);

    void setExtensionsWriter(powsybl::xml::XmlStreamWriter& extensionsWriter);

private:
    powsybl::xml::XmlStreamWriter& m_writer;

    std::reference_wrapper<powsybl::xml::XmlStreamWriter> m_extensionsWriter;

    Anonymizer& m_anonymizer;

    ExportOptions m_options;

    const IidmXmlVersion& m_version;

    std::set<std::string> m_exportedEquipments;

    BusFilter m_filter;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP

