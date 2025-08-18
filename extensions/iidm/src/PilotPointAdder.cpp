/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/PilotPointAdder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

PilotPointAdder::PilotPointAdder(ControlZoneAdder& parent) :
    m_parent(parent) {
}

PilotPointAdder& PilotPointAdder::withBusbarSectionsOrBusesIds(std::vector<std::string> busbarSectionsOrBusesIds) {
    m_busbarSectionsOrBusesIds = busbarSectionsOrBusesIds;
    return *this;
}

PilotPointAdder& PilotPointAdder::withTargetV(double targetV) {
    m_targetV = targetV;
    return *this;
}

ControlZoneAdder& PilotPointAdder::add() {

    if(m_busbarSectionsOrBusesIds.empty()) {
        throw PowsyblException("Empty busbar section of bus ID list");
    }
    for(const auto& id : m_busbarSectionsOrBusesIds) {
        if(id.empty()) {
            throw PowsyblException("Empty busbar section or bus ID");
        }
    }
    if(std::isnan(m_targetV)){
        throw PowsyblException("Invalid target voltage");
    }

    m_parent.withPilotPoint(PilotPoint(m_busbarSectionsOrBusesIds, m_targetV));

    return m_parent;
}

ControlZoneAdder& PilotPointAdder::getParent() {
    return m_parent;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl