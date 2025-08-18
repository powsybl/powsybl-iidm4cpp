/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/extensions/iidm/ControlUnitAdder.hpp>
#include <powsybl/iidm/extensions/iidm/PilotPointAdder.hpp>
#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControlAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ControlZoneAdder::ControlZoneAdder(SecondaryVoltageControlAdder& parent) :
    m_parent(parent) {
}

SecondaryVoltageControlAdder& ControlZoneAdder::getParent() {
    return m_parent;
}

ControlZoneAdder& ControlZoneAdder::withName(const std::string& name) {
    m_name = name;
    return *this;
}

ControlZoneAdder& ControlZoneAdder::withPilotPoint(const PilotPoint& pilotPoint) {
    m_pilotPoint = pilotPoint;
    return *this;
}

ControlZoneAdder& ControlZoneAdder::addControlUnit(const ControlUnit& controlUnit) {
    m_controlUnits.emplace_back(controlUnit); //std::move?
    return *this;
}

PilotPointAdder ControlZoneAdder::newPilotPoint() {
    return PilotPointAdder(*this);
}

ControlUnitAdder ControlZoneAdder::newControlUnit() {
    return ControlUnitAdder(*this);
}


SecondaryVoltageControlAdder& ControlZoneAdder::add() {
    if(m_name.empty()) {
        throw PowsyblException("Zone name is not set");
    }
    if(!m_pilotPoint) {
        throw PowsyblException(stdcxx::format("Pilot point is not set for zone '%1'", m_name));
    }
    if(m_controlUnits.empty()) {
        throw PowsyblException(stdcxx::format("Empty control unit list for zone '%1'", m_name));
    }

    m_parent.addControlZone(ControlZone(m_name, *m_pilotPoint, m_controlUnits));

    return m_parent;
}



}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl