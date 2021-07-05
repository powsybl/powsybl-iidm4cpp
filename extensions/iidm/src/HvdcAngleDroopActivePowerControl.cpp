/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControl.hpp>

#include <cmath>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/HvdcLine.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

HvdcAngleDroopActivePowerControl::HvdcAngleDroopActivePowerControl(HvdcLine& hvdcLine, double p0, double droop, bool enabled) :
    Extension(hvdcLine),
    m_p0(checkP0(p0)),
    m_droop(checkDroop(droop)),
    m_enabled(enabled) {
}

void HvdcAngleDroopActivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<HvdcLine>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<HvdcLine>()));
    }
}

double HvdcAngleDroopActivePowerControl::checkDroop(double droop) {
    if (std::isnan(droop)) {
        throw PowsyblException("droop is not set");
    }
    return droop;
}

double HvdcAngleDroopActivePowerControl::checkP0(double p0) {
    if (std::isnan(p0)) {
        throw PowsyblException("p0 is not set");
    }
    return p0;
}

double HvdcAngleDroopActivePowerControl::getDroop() const {
    return m_droop;
}

const std::string& HvdcAngleDroopActivePowerControl::getName() const {
    static std::string s_name = "hvdcAngleDroopActivePowerControl";
    return s_name;
}

double HvdcAngleDroopActivePowerControl::getP0() const {
    return m_p0;
}

const std::type_index& HvdcAngleDroopActivePowerControl::getType() const {
    static std::type_index s_type = typeid(HvdcAngleDroopActivePowerControl);
    return s_type;
}

bool HvdcAngleDroopActivePowerControl::isEnabled() const {
    return m_enabled;
}

HvdcAngleDroopActivePowerControl& HvdcAngleDroopActivePowerControl::setDroop(double droop) {
    m_droop = checkDroop(droop);
    return *this;
}

HvdcAngleDroopActivePowerControl& HvdcAngleDroopActivePowerControl::setEnabled(bool enabled) {
    m_enabled = enabled;
    return *this;
}

HvdcAngleDroopActivePowerControl& HvdcAngleDroopActivePowerControl::setP0(double p0) {
    m_p0 = checkP0(p0);
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
