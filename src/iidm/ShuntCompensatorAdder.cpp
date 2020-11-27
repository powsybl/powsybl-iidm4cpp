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

    std::unique_ptr<ShuntCompensatorModel> model;
    if (m_shuntCompensatorLinearModelAdder.is_initialized()) {
        model = stdcxx::make_unique<ShuntCompensatorLinearModel>(m_shuntCompensatorLinearModelAdder->m_bPerSection,
                                                                 m_shuntCompensatorLinearModelAdder->m_gPerSection,
                                                                 *m_shuntCompensatorLinearModelAdder->m_maximumSectionCount);
    } else if (m_shuntCompensatorNonLinearModelAdder.is_initialized()) {
        model = stdcxx::make_unique<ShuntCompensatorNonLinearModel>(m_shuntCompensatorNonLinearModelAdder->m_sections);
    }

    if (!model) {
        throw ValidationException(*this, "the shunt compensator model has not been defined");
    }

    if (!m_sectionCount.is_initialized()) {
        throw ValidationException(*this, "section count is not set");
    }

    checkSections(*this, *m_sectionCount, model->getMaximumSectionCount());

    auto ptrTerminal = checkAndGetTerminal();
    Terminal& regulatingTerminal = m_regulatingTerminal ? m_regulatingTerminal.get() : *ptrTerminal;
    std::unique_ptr<ShuntCompensator> ptrShunt = stdcxx::make_unique<ShuntCompensator>(getNetwork(), checkAndGetUniqueId(), getName(), isFictitious(), std::move(model),
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
    return ShuntCompensatorAdder::ShuntCompensatorLinearModelAdder(*this);
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
