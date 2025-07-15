/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BatteryAdder.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>


namespace powsybl {

namespace iidm {

BatteryAdder::BatteryAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

Battery& BatteryAdder::add() {
    Network& network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkP0(*this, m_p0, network.getMinimumValidationLevel()));
    network.setValidationLevelIfGreaterThan(checkQ0(*this, m_q0, network.getMinimumValidationLevel()));
    checkMinP(*this, m_minP);
    checkMaxP(*this, m_maxP);
    checkActivePowerLimits(*this, m_minP, m_maxP);

    std::unique_ptr<Battery> ptrBattery = stdcxx::make_unique<Battery>(network, checkAndGetUniqueId(), getName(), isFictitious(), m_p0, m_q0, m_minP, m_maxP);
    auto& battery = network.checkAndAdd<Battery>(std::move(ptrBattery));

    Terminal& terminal = battery.addTerminal(checkAndGetTerminal());
    getVoltageLevel().attach(terminal, false);

    return battery;
}

const std::string& BatteryAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Battery";

    return s_typeDescription;
}

BatteryAdder& BatteryAdder::setMaxP(double maxP) {
    m_maxP = maxP;
    return *this;
}

BatteryAdder& BatteryAdder::setMinP(double minP) {
    m_minP = minP;
    return *this;
}

BatteryAdder& BatteryAdder::setP0(double p0) {
    m_p0 = p0;
    return *this;
}

BatteryAdder& BatteryAdder::setQ0(double q0) {
    m_q0 = q0;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
