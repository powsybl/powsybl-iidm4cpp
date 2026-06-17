/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/TopologyLevelXmlUtil.hpp>

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/iidm/converter/xml/NetworkXmlWriterContext.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

TopologyLevel TopologyLevelXmlUtil::determineTopologyLevel(const VoltageLevel& voltageLevel, NetworkXmlWriterContext& context) {
    auto exportTopologyLevel = context.getVoltageLevelTopologyLevel(voltageLevel.getId());
    if(!exportTopologyLevel.has_value()) { // check has not been performed on this voltage level yet
        auto configTopologyLevel = context.getOptions().getVoltageLevelTopologyLevel(voltageLevel.getId());
        if(!configTopologyLevel.has_value()) {
            configTopologyLevel = context.getOptions().getTopologyLevel();
        }
        TopologyLevel expectedTopologyLevel = getMinTopologyLevel(voltageLevel.getTopologyKind(), configTopologyLevel.get());

        exportTopologyLevel = checkVoltageLevelExportTopology(voltageLevel, context, expectedTopologyLevel);
        context.addVoltageLevelExportTopologyLevel(voltageLevel.getId(),  exportTopologyLevel.get());
    }
    return exportTopologyLevel.get();

}

TopologyLevel TopologyLevelXmlUtil::checkVoltageLevelExportTopology(const VoltageLevel& voltageLevel, const NetworkXmlWriterContext& context, const TopologyLevel& topologyLevel) {
    if(topologyLevel != TopologyLevel::BUS_BRANCH) {
        return topologyLevel;
    }

    for (const auto& connectable : voltageLevel.getConnectables()) {
        for (const auto& terminal : connectable.getTerminals()) {
            if( !terminal.get().getBusView().getConnectableBus()) {  
                // At least one terminal which has a missing connectable bus reference:

                if (context.getOptions().getBusBranchVoltageLevelIncompatibilityBehavior() == ExportOptions::BusBranchVoltageLevelIncompatibilityBehavior::THROW_EXCEPTION) { 
                    throw PowsyblException(stdcxx::format("Cannot export voltage level '%1%' in BUS_BRANCH topology: this would lead to an invalid IIDM.", voltageLevel.getId()));
                }
                return getMinTopologyLevel(voltageLevel.getTopologyKind(), TopologyLevel::NODE_BREAKER);

            }
        }
    }
    //No issue detected:
    return topologyLevel;
}


}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
