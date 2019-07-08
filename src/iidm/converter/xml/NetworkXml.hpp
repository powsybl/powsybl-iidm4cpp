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

namespace powsybl {

namespace iidm {

class Network;

namespace converter {

class Anonymizer;
class ExportOptions;
class ImportOptions;

namespace xml {

class NetworkXml {
public:
    static Network read(std::istream& is, const ImportOptions& options, const Anonymizer& anonymizer);

    static std::unique_ptr<Anonymizer> write(std::ostream& ostream, const Network& network, const ExportOptions& options);

public:
    NetworkXml() = delete;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NETWORKXML_HPP
