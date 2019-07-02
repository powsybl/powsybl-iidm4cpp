/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP

#include <set>
#include <string>

#include <powsybl/iidm/converter/ExportOptions.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

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
    NetworkXmlWriterContext(const stdcxx::reference_wrapper<Anonymizer>& anonymizer, powsybl::xml::XmlStreamWriter& writer, const ExportOptions& options);

    ~NetworkXmlWriterContext() = default;

    void addExportedEquipment(const Identifiable& identifiable);

    const stdcxx::reference_wrapper<Anonymizer>& getAnonymizer() const;

    stdcxx::reference_wrapper<Anonymizer>& getAnonymizer();

    const std::set<std::string>& getExportedEquipments() const;

    const ExportOptions& getOptions() const;

    powsybl::xml::XmlStreamWriter& getWriter();

private:
    powsybl::xml::XmlStreamWriter& m_writer;

    stdcxx::reference_wrapper<Anonymizer> m_anonymizer;

    ExportOptions m_options;

    std::set<std::string> m_exportedEquipments;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP

