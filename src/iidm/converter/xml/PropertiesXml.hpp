/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HPP

namespace powsybl {

namespace iidm {

class Identifiable;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class PropertiesXml {
public:
    static void read(Identifiable& identifiable, NetworkXmlReaderContext& context);

    static void write(const Identifiable& identifiable, NetworkXmlWriterContext& context);

private:
    PropertiesXml() = default;

    ~PropertiesXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_PROPERTIESXML_HPP
