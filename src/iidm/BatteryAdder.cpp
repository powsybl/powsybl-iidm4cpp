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
    checkP0(*this, m_targetP);
    checkQ0(*this, m_targetQ);
    checkMinP(*this, m_minP);
    checkMaxP(*this, m_maxP);
    checkActivePowerLimits(*this, m_minP, m_maxP);

    std::unique_ptr<Battery> ptrBattery = stdcxx::make_unique<Battery>(getNetwork(), checkAndGetUniqueId(), getName(), isFictitious(), m_targetP, m_targetQ, m_minP, m_maxP);
    auto& battery = getNetwork().checkAndAdd<Battery>(std::move(ptrBattery));

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

BatteryAdder& BatteryAdder::setTargetP(double targetP) {
    m_targetP = targetP;
    return *this;
}

BatteryAdder& BatteryAdder::setTargetQ(double targetQ) {
    m_targetQ = targetQ;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
