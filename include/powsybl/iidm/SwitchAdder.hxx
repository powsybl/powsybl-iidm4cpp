/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SWITCHADDER_HXX
#define POWSYBL_IIDM_SWITCHADDER_HXX

#include <powsybl/iidm/SwitchAdder.hpp>

namespace powsybl {

namespace iidm {

template <typename SwitchAdder>
AbstractSwitchAdder<SwitchAdder>::AbstractSwitchAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel) {
}

template <typename SwitchAdder>
const Network& AbstractSwitchAdder<SwitchAdder>::getNetwork() const {
    return getVoltageLevel().getNetwork();
}

template <typename SwitchAdder>
Network& AbstractSwitchAdder<SwitchAdder>::getNetwork() {
    return getVoltageLevel().getNetwork();
}

template <typename SwitchAdder>
const VoltageLevel& AbstractSwitchAdder<SwitchAdder>::getVoltageLevel() const {
    return m_voltageLevel;
}


template <typename SwitchAdder>
VoltageLevel& AbstractSwitchAdder<SwitchAdder>::getVoltageLevel() {
    return m_voltageLevel;
}

template <typename SwitchAdder>
const std::string& AbstractSwitchAdder<SwitchAdder>::getTypeDescription() const {
    static std::string s_typeDescription = "Switch";

    return s_typeDescription;
}

template <typename SwitchAdder>
bool AbstractSwitchAdder<SwitchAdder>::isOpen() const {
    return m_open;
}

template <typename SwitchAdder>
SwitchAdder& AbstractSwitchAdder<SwitchAdder>::setOpen(bool open) {
    m_open = open;
    return static_cast<SwitchAdder&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SWITCHADDER_HXX
