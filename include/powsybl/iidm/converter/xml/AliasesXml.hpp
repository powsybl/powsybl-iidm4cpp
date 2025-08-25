/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_ALIASESXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_ALIASESXML_HPP

#include <functional>
#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class AliasesXml {
public:
    static void read(Identifiable& identifiable, const NetworkXmlReaderContext& context);

    static void read(std::vector<std::function<void(Identifiable&)>>& toApply, const NetworkXmlReaderContext& context);

    static void write(const Identifiable& identifiable, const std::string& rootElementName, NetworkXmlWriterContext& context);

private:

    static std::function<void(Identifiable&)> read(const NetworkXmlReaderContext& context);

    AliasesXml() = default;

    ~AliasesXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_ALIASESXML_HPP
