/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VscConverterStationAdder.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VscConverterStation.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

VscConverterStationAdder::VscConverterStationAdder(VoltageLevel& voltageLevel) :
    HvdcConverterStationAdder(voltageLevel) {
}

VscConverterStation& VscConverterStationAdder::add() {
    validate();

    auto terminalPtr = checkAndGetTerminal();
    Terminal& regulatingTerminal = m_regulatingTerminal ? m_regulatingTerminal.get() : *terminalPtr;
    std::unique_ptr<VscConverterStation> ptrVsc = stdcxx::make_unique<VscConverterStation>(getNetwork(), checkAndGetUniqueId(), getName(), isFictitious(), getLossFactor(), *m_voltageRegulatorOn, m_reactivePowerSetpoint, m_voltageSetpoint, regulatingTerminal);
    auto& vsc = getNetwork().checkAndAdd<VscConverterStation>(std::move(ptrVsc));

    Terminal& terminal = vsc.addTerminal(std::move(terminalPtr));
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

VscConverterStationAdder& VscConverterStationAdder::setRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal) {
    m_regulatingTerminal = terminal;
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
    checkRegulatingTerminal(*this, m_regulatingTerminal, getNetwork());
}

}  // namespace iidm

}  // namespace powsybl
