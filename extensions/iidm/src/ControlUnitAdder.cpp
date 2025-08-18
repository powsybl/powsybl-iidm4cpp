/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ControlUnitAdder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ControlUnitAdder::ControlUnitAdder(ControlZoneAdder& parent) :
    m_parent(parent) {
}

ControlUnitAdder& ControlUnitAdder::withId(const std::string& id) {
    m_id = id;
    return *this;
}

ControlUnitAdder& ControlUnitAdder::withParticipate(bool participate) {
    m_participate = participate;
    return *this;
}

ControlZoneAdder& ControlUnitAdder::add() {

    if(m_id.empty()) {
        throw PowsyblException("Control unit ID is not set");
    }

    m_parent.addControlUnit(ControlUnit(m_id, m_participate));

    return m_parent;
}

ControlZoneAdder& ControlUnitAdder::getParent() {
    return m_parent;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl