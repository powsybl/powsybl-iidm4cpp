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
     * Once a first check has been performed for a given VoltageLevel, the result is stored in the NetworkXmlWriterContext for further call of this function
     */
    static TopologyLevel determineTopologyLevel(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context);

public:
    TopologyLevelXmlUtil() = delete;

private:


    /**
     * Check and possibly adjusts the topologyLevel to use for the export of the given Voltage Level
     * 
     * If the requested topologyLevel is TopologyLevel::BUS_BRANCH and the provided VoltageLevel has switches,
     * this method checks whether this voltage level can be exported without leading to an invalid IIDM
     * 
     * The resulting IIDM could be invalid when an equipment references a bus that has not been exported.
     * This may happen with connectables that are not connected, if their connectable bus has nothong connected to it
     * 
     *  When a potential problem is detected, the behavior is driven by the export options
     * available in the provided context:
     * - If the configured BusBranchVoltageLevelIncompatibilityBehavior is THROW_EXCEPTION, a PowsyblException is thrown.
     * - Otherwise, the method falls back to the voltage level's own topology kind.
     *
     * If the requested topologyLevel is not BUS_BRANCH the original requested topologyLevel is returned unchanged.
     * 
     * @param voltageLevel The Voltage Level to export
     * @param context the current NetworkXmlWriterContext export context
     * @param topologyLevel the requested export TopologyLevel for this VoltageLevel
     * 
     */
    static TopologyLevel checkVoltageLevelExportTopology(const VoltageLevel& voltageLevel, const NetworkXmlWriterContext& context, const TopologyLevel& topologyLevel);
};

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_TOPOLOGYLEVELXMLUTIL_HPP
