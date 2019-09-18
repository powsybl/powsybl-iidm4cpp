/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWINTERNALCONNECTIONXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWINTERNALCONNECTIONXML_HPP

namespace powsybl {

namespace iidm {

class VoltageLevel;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class NodeBreakerViewInternalConnectionXml {
public:
    static const NodeBreakerViewInternalConnectionXml& getInstance();

public:
    void read(VoltageLevel& voltageLevel, const NetworkXmlReaderContext& context) const;

    void write(unsigned long node1, unsigned long node2, NetworkXmlWriterContext& context) const;

private:
    NodeBreakerViewInternalConnectionXml() = default;

    ~NodeBreakerViewInternalConnectionXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_NODEBREAKERVIEWINTERNALCONNECTIONXML_HPP

