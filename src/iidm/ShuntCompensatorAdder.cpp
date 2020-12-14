/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorAdder.hpp>

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

ShuntCompensatorAdder::ShuntCompensatorAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

ShuntCompensator& ShuntCompensatorAdder::add() {
    checkRegulatingTerminal(*this, m_regulatingTerminal, getNetwork());
    checkVoltageControl(*this, m_voltageRegulatorOn, m_targetV);
    checkTargetDeadband(*this, "shunt compensator", m_voltageRegulatorOn, m_targetDeadband);

    if (!m_modelBuilder) {
        throw ValidationException(*this, "the shunt compensator model has not been defined");
    }
    checkSections(*this, m_sectionCount, m_modelBuilder->getMaximumSectionCount());

    auto ptrTerminal = checkAndGetTerminal();
    Terminal& regulatingTerminal = m_regulatingTerminal ? m_regulatingTerminal.get() : *ptrTerminal;
    std::unique_ptr<ShuntCompensator> ptrShunt = stdcxx::make_unique<ShuntCompensator>(getNetwork(), checkAndGetUniqueId(), getName(), isFictitious(), m_modelBuilder->build(),
                                                                                       *m_sectionCount, regulatingTerminal, m_voltageRegulatorOn, m_targetV, m_targetDeadband);
    auto& shunt = getNetwork().checkAndAdd<ShuntCompensator>(std::move(ptrShunt));

    Terminal& terminal = shunt.addTerminal(std::move(ptrTerminal));
    getVoltageLevel().attach(terminal, false);

    return shunt;
}

const std::string& ShuntCompensatorAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Shunt compensator";

    return s_typeDescription;
}

ShuntCompensatorAdder::ShuntCompensatorLinearModelAdder ShuntCompensatorAdder::newLinearModel() {
    return ShuntCompensatorLinearModelAdder(*this);
}

ShuntCompensatorAdder::ShuntCompensatorNonLinearModelAdder ShuntCompensatorAdder::newNonLinearModel() {
    return ShuntCompensatorNonLinearModelAdder(*this);
}

ShuntCompensatorAdder& ShuntCompensatorAdder::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    m_regulatingTerminal = regulatingTerminal;
    return *this;
}

ShuntCompensatorAdder& ShuntCompensatorAdder::setSectionCount(unsigned long sectionCount) {
    m_sectionCount = sectionCount;
    return *this;
}

void ShuntCompensatorAdder::setShuntCompensatorModelBuilder(std::unique_ptr<ShuntCompensatorModelAdder>&& adder) {
    m_modelBuilder = std::move(adder);
}

ShuntCompensatorAdder& ShuntCompensatorAdder::setTargetDeadband(double targetDeadband) {
    m_targetDeadband = targetDeadband;
    return *this;
}

ShuntCompensatorAdder& ShuntCompensatorAdder::setTargetV(double targetV) {
    m_targetV = targetV;
    return *this;
}

ShuntCompensatorAdder& ShuntCompensatorAdder::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn = voltageRegulatorOn;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
