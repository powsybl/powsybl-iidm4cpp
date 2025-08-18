/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONE_HPP

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>
#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ControlZone {
public:

    ControlZone(const std::string& name, const PilotPoint& pilotPoint, const std::vector<ControlUnit> controlUnits);

    ControlZone() = delete;
    ControlZone(const ControlZone& coord) = default;
    ControlZone(ControlZone&&) noexcept = default;
    ~ControlZone() noexcept = default;
    ControlZone& operator=(const ControlZone&) = default;
    ControlZone& operator=(ControlZone&&) noexcept = default;

    const std::string& getName() const;
    const PilotPoint& getPilotPoint() const;
    PilotPoint& getPilotPoint();
    const std::vector<ControlUnit>& getControlUnits() const;

private:

    std::string m_name;

    PilotPoint m_pilotPoint;

    std::vector<ControlUnit> m_controlUnits;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLZONE_HPP
