/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NETWORKXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NETWORKXML_HPP

#include <iosfwd>
#include <memory>
#include <string>

namespace powsybl {

namespace iidm {

class Network;

namespace converter {

class Anonymizer;
class BusFilter;
class ExportOptions;
class ImportOptions;

namespace xml {

class NetworkXmlWriterContext;

class NetworkXml {
public:
    static Network read(const std::string& filename, std::istream& is, const ImportOptions& options);

    static void write(const std::string& filename, std::ostream& os, const Network& network, const ExportOptions& options);

public:
    NetworkXml() = delete;

private:
    static void writeHvdcLines(const BusFilter& filter, const Network& network, NetworkXmlWriterContext& context);

    static void writeLines(const BusFilter& filter, const Network& network, NetworkXmlWriterContext& context);

    static void writeSubstations(const Network& network, NetworkXmlWriterContext& context);

    static void writeTransformers(const BusFilter& filter, const Network& network, NetworkXmlWriterContext& context);

    static void writeVoltageLevels(const Network& network, NetworkXmlWriterContext& context);
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXML_HPP
