/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VOLTAGEANGLELIMITXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VOLTAGEANGLELIMITXML_HPP

#include <powsybl/iidm/VoltageAngleLimit.hpp>
#include <powsybl/iidm/VoltageAngleLimitAdder.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class VoltageAngleLimitXml {
public:
    static const VoltageAngleLimitXml& getInstance();

    void read(Network& network, NetworkXmlReaderContext& context) const;

    void write(const VoltageAngleLimit& limit, const Network& network, NetworkXmlWriterContext& context) const;

private:
    VoltageAngleLimitXml() = default;

    ~VoltageAngleLimitXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VOLTAGEANGLELIMITXML_HPP

