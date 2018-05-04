/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SWITCHADDER_HXX
#define POWSYBL_IIDM_SWITCHADDER_HXX

#include <powsybl/iidm/SwitchAdder.hpp>

#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

template <typename SwitchAdder>
AbstractSwitchAdder<SwitchAdder>::AbstractSwitchAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel),
    m_open(false),
    m_fictitious(false) {
}

template <typename SwitchAdder>
Network& AbstractSwitchAdder<SwitchAdder>::getNetwork() {
    return m_voltageLevel.getNetwork();
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
bool AbstractSwitchAdder<SwitchAdder>::isFictitious() const {
    return m_fictitious;
}

template <typename SwitchAdder>
bool AbstractSwitchAdder<SwitchAdder>::isOpen() const {
    return m_open;
}

template <typename SwitchAdder>
SwitchAdder& AbstractSwitchAdder<SwitchAdder>::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
    return static_cast<SwitchAdder&>(*this);
}

template <typename SwitchAdder>
SwitchAdder& AbstractSwitchAdder<SwitchAdder>::setOpen(bool open) {
    m_open = open;
    return static_cast<SwitchAdder&>(*this);
}

}

}

#endif  // POWSYBL_IIDM_SWITCHADDER_HXX
