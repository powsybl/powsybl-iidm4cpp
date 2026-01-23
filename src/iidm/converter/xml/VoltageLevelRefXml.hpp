/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELREFXML_HPP
#define POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELREFXML_HPP

#include <string>
#include <functional>

namespace powsybl {

namespace iidm {

class Network;
class VoltageLevel;

namespace converter {

namespace xml {

class NetworkXmlReaderContext;
class NetworkXmlWriterContext;

class VoltageLevelRefXml {
public:
    static void readVoltageLevelRef(Network& network, NetworkXmlReaderContext& context, const std::function<void(VoltageLevel&)>& endTaskTerminalConsumer);

    static void writeVoltageLevelRef(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context);

private:
    VoltageLevelRefXml() = default;

    ~VoltageLevelRefXml() noexcept = default;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_VOLTAGELEVELREFXML_HPP

