/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNITADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNITADDER_HPP

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ControlZoneAdder;

class ControlUnitAdder {
public:

    ControlUnitAdder(ControlZoneAdder& parent);

    ~ControlUnitAdder() noexcept = default;
    ControlUnitAdder(const ControlUnitAdder&) = default;
    ControlUnitAdder(ControlUnitAdder&&) = default;
    ControlUnitAdder& operator=(const ControlUnitAdder&) = delete;
    ControlUnitAdder& operator=(ControlUnitAdder&&) = delete;

    //creates the control unit and add it into parent's member vector data
    ControlZoneAdder& add();
    ControlZoneAdder& getParent();

    ControlUnitAdder& withId(const std::string& id);

    ControlUnitAdder& withParticipate(bool participate);

private:
    std::string m_id;

    bool m_participate = true;

    ControlZoneAdder& m_parent;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_CONTROLUNITADDER_HPP