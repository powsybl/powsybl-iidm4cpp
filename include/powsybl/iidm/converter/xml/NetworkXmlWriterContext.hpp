/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP

#include <functional>
#include <memory>
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

static const int MAX_NAMESPACE_PREFIX_NUM = 100;

class NetworkXmlWriterContext {
public:
    NetworkXmlWriterContext(std::unique_ptr<Anonymizer>&& anonymizer, powsybl::xml::XmlStreamWriter& writer, const ExportOptions& options, const BusFilter& filter, const IidmXmlVersion& version = IidmXmlVersion::CURRENT_IIDM_XML_VERSION(), bool valid = true);

    ~NetworkXmlWriterContext() = default;

    void addExportedEquipment(const Identifiable& identifiable);

    const Anonymizer& getAnonymizer() const;

    Anonymizer& getAnonymizer();

    const std::set<std::string>& getExportedEquipments() const;

    const std::string& getExtensionVersion(const std::string& extensionName) const;

    const BusFilter& getFilter() const;

    const ExportOptions& getOptions() const;

    const IidmXmlVersion& getVersion() const;

    bool isValid() const;

    powsybl::xml::XmlStreamWriter& getWriter();

    bool isExportedEquipment(const std::string& id);

    /**
     * Store a "fixed" prefix to be associated with the given extension, to be used in place of the default one defined in the serializer
     * Only relevant in case several extension would use the same prefix
     */
    void addExtensionFixedPrefix(const std::string& extensionName, const std::string& fixedPrefix);
    /**
     * Get the fixed prefix associated with the given extension name.
     * If extension not found returns an empty string.
     */
    std::string getExtensionFixedPrefix(const std::string& extensionName) const;

    void addVoltageLevelExportTopologyLevel(const std::string& voltageLevelId, const TopologyLevel& topologyLevel);

    stdcxx::optional<TopologyLevel> getVoltageLevelTopologyLevel(const std::string& voltageLevelId) const;

private:
    powsybl::xml::XmlStreamWriter& m_writer;

    std::unique_ptr<Anonymizer> m_anonymizer;

    ExportOptions m_options;

    const IidmXmlVersion& m_version;

    std::set<std::string> m_exportedEquipments;

    BusFilter m_filter;

    bool m_valid;

    std::map<std::string, std::string> m_fixedExtensionPrefixes;

    std::map<std::string, TopologyLevel> m_voltageLevelExportTopologyLevels;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXMLWRITERCONTEXT_HPP

