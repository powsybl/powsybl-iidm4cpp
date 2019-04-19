/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/GeneratorAdder.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

GeneratorAdder::GeneratorAdder(powsybl::iidm::VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel),
    m_energySource(EnergySource::OTHER),
    m_minP(stdcxx::nan()),
    m_maxP(stdcxx::nan()),
    m_ratedS(stdcxx::nan()),
    m_activePowerSetpoint(stdcxx::nan()),
    m_reactivePowerSetpoint(stdcxx::nan()),
    m_voltageSetpoint(stdcxx::nan()) {
}

Generator& GeneratorAdder::add() {
    checkMinP(*this, m_minP);
    checkMaxP(*this, m_maxP);
    if (m_regulatingTerminal) {
        checkRegulatingTerminal(*this, m_regulatingTerminal, getNetwork());
    }
    checkActivePowerSetpoint(*this, m_activePowerSetpoint);

    checkOptional(*this, m_voltageRegulatorOn, "voltage regulator status is not set");
    checkVoltageControl(*this, *m_voltageRegulatorOn, m_voltageSetpoint, m_reactivePowerSetpoint);
    checkActiveLimits(*this, m_minP, m_maxP);
    checkRatedS(*this, m_ratedS);

    std::unique_ptr<Generator> ptrGenerator = stdcxx::make_unique<Generator>(getNetwork(), getId(), getName(),
        m_energySource, m_minP, m_maxP, *m_voltageRegulatorOn, m_regulatingTerminal,
        m_activePowerSetpoint, m_reactivePowerSetpoint, m_voltageSetpoint, m_ratedS);
    auto& generator = getNetwork().checkAndAdd(std::move(ptrGenerator));

    Terminal& terminal = generator.addTerminal(getTerminal());
    m_voltageLevel.attach(terminal, false);

    return generator;
}

Network& GeneratorAdder::getNetwork() {
    return m_voltageLevel.getNetwork();
}

const std::string& GeneratorAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Generator";

    return s_typeDescription;

}

GeneratorAdder& GeneratorAdder::setActivePowerSetpoint(double activePowerSetpoint) {
    m_activePowerSetpoint = activePowerSetpoint;
    return *this;
}

GeneratorAdder& GeneratorAdder::setEnergySource(const EnergySource& energySource) {
    m_energySource = energySource;
    return *this;
}

GeneratorAdder& GeneratorAdder::setMaxP(double maxP) {
    m_maxP = maxP;
    return *this;
}

GeneratorAdder& GeneratorAdder::setMinP(double minP) {
    m_minP = minP;
    return *this;
}

GeneratorAdder& GeneratorAdder::setRatedS(double ratedS) {
    m_ratedS = ratedS;
    return *this;
}

GeneratorAdder& GeneratorAdder::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    m_reactivePowerSetpoint = reactivePowerSetpoint;
    return *this;
}

GeneratorAdder& GeneratorAdder::setRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal) {
    m_regulatingTerminal = terminal;
    return *this;
}

GeneratorAdder& GeneratorAdder::setTargetP(double activePowerSetpoint) {
    return setActivePowerSetpoint(activePowerSetpoint);
}

GeneratorAdder& GeneratorAdder::setTargetQ(double reactivePowerSetpoint) {
    return setReactivePowerSetpoint(reactivePowerSetpoint);
}

GeneratorAdder& GeneratorAdder::setTargetV(double voltageSetpoint) {
    return setVoltageSetpoint(voltageSetpoint);
}

GeneratorAdder& GeneratorAdder::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn = voltageRegulatorOn;
    return *this;
}

GeneratorAdder& GeneratorAdder::setVoltageSetpoint(double voltageSetpoint) {
    m_voltageSetpoint = voltageSetpoint;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
