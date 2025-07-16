/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/util/VoltageLevels.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/IdentifiableType.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace VoltageLevels {

void checkRemovability(const VoltageLevel& voltageLevel) {
    const Network& network = voltageLevel.getNetwork();
    for (const Connectable& connectable : voltageLevel.getConnectables()) {
        const IdentifiableType& type = connectable.getType();
        if (MULTIPLE_TERMINALS_CONNECTABLE_TYPES.find(type) != MULTIPLE_TERMINALS_CONNECTABLE_TYPES.end()) {
            // Reject lines, 2WT and 3WT
            throw AssertionError(stdcxx::format("The voltage level '%1%' cannot be removed because of a remaining %2%", voltageLevel.getId(), type));
        }
        if (type == IdentifiableType::HVDC_CONVERTER_STATION && network.findHvdcLine(dynamic_cast<const HvdcConverterStation&>(connectable))) {
            // Reject all converter stations connected to a HVDC line
            throw AssertionError(stdcxx::format("The voltage level '%1%' cannot be removed because of a remaining HVDC line", voltageLevel.getId()));
        }
    }
}

}  // namespace VoltageLevels

}  // namespace iidm

}  // namespace powsybl
