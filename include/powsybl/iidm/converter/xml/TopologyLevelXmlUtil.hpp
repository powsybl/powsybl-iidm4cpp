/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_TOPOLOGYLEVELXMLUTIL_HPP
#define POWSYBL_IIDM_CONVERTER_XML_TOPOLOGYLEVELXMLUTIL_HPP

#include "powsybl/iidm/TopologyLevel.hpp"

namespace powsybl {

namespace iidm {

class VoltageLevel;

namespace converter {

namespace xml {

class NetworkXmlWriterContext;

class TopologyLevelXmlUtil {
public:
    /**
     * Determine the TopologyLevel to be used for the serialization of the given VoltageLevel, depending on the NNetworkXmlWriterContext's options
     */
    static TopologyLevel determineTopologyLevel(const VoltageLevel& voltageLevel, const NetworkXmlWriterContext& context);

public:
    TopologyLevelXmlUtil() = delete;
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TOPOLOGYLEVELXMLUTIL_HPP
