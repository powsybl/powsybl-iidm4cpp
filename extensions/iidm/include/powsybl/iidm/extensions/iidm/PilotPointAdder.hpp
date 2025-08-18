/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_PILOTPOINTADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_PILOTPOINTADDER_HPP

#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ControlZoneAdder;

class PilotPointAdder {
public:

    PilotPointAdder(ControlZoneAdder& parent);

    ~PilotPointAdder() noexcept = default;
    PilotPointAdder(const PilotPointAdder&) = default;
    PilotPointAdder(PilotPointAdder&&) = default;
    PilotPointAdder& operator=(const PilotPointAdder&) = delete;
    PilotPointAdder& operator=(PilotPointAdder&&) = delete;


    //creates the pilot point and set it into it's parent member data
    ControlZoneAdder& add();
    ControlZoneAdder& getParent();

    PilotPointAdder& withBusbarSectionsOrBusesIds(std::vector<std::string> busbarSectionsOrBusesIds);

    PilotPointAdder& withTargetV(double targetV);

private:
    ControlZoneAdder& m_parent;

    std::vector<std::string> m_busbarSectionsOrBusesIds;

    double m_targetV = stdcxx::nan();
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNITADDER_HPP