/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/iidm/VscConverterStationAdder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

VscConverterStationAdder::VscConverterStationAdder(VoltageLevel& voltageLevel) :
    HvdcConverterStationAdder(voltageLevel) {
}

VscConverterStation& VscConverterStationAdder::add() {
    validate();

    std::unique_ptr<VscConverterStation> ptrVsc = stdcxx::make_unique<VscConverterStation>(getNetwork(), checkAndGetUniqueId(), getName(), getLossFactor(), *m_voltageRegulatorOn, m_reactivePowerSetpoint, m_voltageSetpoint);
    auto& vsc = getNetwork().checkAndAdd<VscConverterStation>(std::move(ptrVsc));

    Terminal& terminal = vsc.addTerminal(checkAndGetTerminal());
    getVoltageLevel().attach(terminal, false);

    return vsc;
}

const std::string& VscConverterStationAdder::getTypeDescription() const {
    static std::string s_typeDescription = "vscConverterStation";

    return s_typeDescription;
}

VscConverterStationAdder& VscConverterStationAdder::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    m_reactivePowerSetpoint = reactivePowerSetpoint;
    return *this;
}

VscConverterStationAdder& VscConverterStationAdder::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn = voltageRegulatorOn;
    return *this;
}

VscConverterStationAdder& VscConverterStationAdder::setVoltageSetpoint(double voltageSetpoint) {
    m_voltageSetpoint = voltageSetpoint;
    return *this;
}

void VscConverterStationAdder::validate() const {
    HvdcConverterStationAdder::validate();
    checkOptional(*this, m_voltageRegulatorOn, "voltage regulator status is not set");
    checkVoltageControl(*this, *m_voltageRegulatorOn, m_voltageSetpoint, m_reactivePowerSetpoint);
}

}  // namespace iidm

}  // namespace powsybl
