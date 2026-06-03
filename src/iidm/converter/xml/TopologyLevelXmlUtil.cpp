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

TopologyLevel TopologyLevelXmlUtil::determineTopologyLevel(const VoltageLevel& voltageLevel, const NetworkXmlWriterContext& context) {
    auto configTopologyLevel = context.getOptions().getVoltageLevelTopologyLevel(voltageLevel.getId());
    if(!configTopologyLevel.has_value()) {
        configTopologyLevel = context.getOptions().getTopologyLevel();
    }

    return getMinTopologyLevel(voltageLevel.getTopologyKind(), configTopologyLevel.get());
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
