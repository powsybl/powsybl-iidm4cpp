/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONEADDER_HPP

#include <powsybl/iidm/extensions/iidm/ControlZone.hpp>

#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class SecondaryVoltageControlAdder; 
class ControlUnitAdder;
class PilotPointAdder;

class ControlZoneAdder {
public:
    ControlZoneAdder(SecondaryVoltageControlAdder& parent);

    ~ControlZoneAdder() noexcept = default;
    ControlZoneAdder(const ControlZoneAdder&) = default;
    ControlZoneAdder(ControlZoneAdder&&) = default;
    ControlZoneAdder& operator=(const ControlZoneAdder&) = delete;
    ControlZoneAdder& operator=(ControlZoneAdder&&) = delete;

    //creates the control zone and add it into parent's member vector data
    SecondaryVoltageControlAdder& add();
    SecondaryVoltageControlAdder& getParent();


    ControlZoneAdder& withName(const std::string& name);

    ControlZoneAdder& withPilotPoint(const PilotPoint& pilotPoint);
    //retrieve a new PilotPointAdder
    PilotPointAdder newPilotPoint();

    ControlZoneAdder& addControlUnit(const ControlUnit& controlUnit);
    //retrieve a new ControlUnitAdder
    ControlUnitAdder newControlUnit();

private:

    std::string m_name;

    stdcxx::optional<PilotPoint> m_pilotPoint;

    std::vector<ControlUnit> m_controlUnits;

    SecondaryVoltageControlAdder& m_parent;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONEADDER_HPP