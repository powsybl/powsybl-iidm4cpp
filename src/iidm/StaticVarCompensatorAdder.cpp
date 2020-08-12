/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/StaticVarCompensatorAdder.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

StaticVarCompensatorAdder::StaticVarCompensatorAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

StaticVarCompensator& StaticVarCompensatorAdder::add() {
    checkBmin(*this, m_bMin);
    checkBmax(*this, m_bMax);
    checkRegulatingTerminal(*this, m_regulatingTerminal, getNetwork());
    checkSvcRegulator(*this, m_voltageSetpoint, m_reactivePowerSetpoint, m_regulationMode);

    std::unique_ptr<Terminal> ptrTerminal = checkAndGetTerminal();
    Terminal& regulatingTerminal = m_regulatingTerminal ? m_regulatingTerminal.get() : *ptrTerminal;
    std::unique_ptr<StaticVarCompensator> ptrSvc = stdcxx::make_unique<StaticVarCompensator>(getNetwork(), checkAndGetUniqueId(), getName(), isFictitious(), m_bMin, m_bMax, m_voltageSetpoint,
        m_reactivePowerSetpoint, *m_regulationMode, regulatingTerminal);
    auto& svc = getNetwork().checkAndAdd<StaticVarCompensator>(std::move(ptrSvc));

    Terminal& terminal = svc.addTerminal(std::move(ptrTerminal));
    getVoltageLevel().attach(terminal, false);

    return svc;
}

const std::string& StaticVarCompensatorAdder::getTypeDescription() const {
    static std::string s_typeDescription = "staticVarCompensator";

    return s_typeDescription;
}

StaticVarCompensatorAdder& StaticVarCompensatorAdder::setBmax(double bMax) {
    m_bMax = bMax;
    return *this;
}

StaticVarCompensatorAdder& StaticVarCompensatorAdder::setBmin(double bMin) {
    m_bMin = bMin;
    return *this;
}

StaticVarCompensatorAdder& StaticVarCompensatorAdder::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    m_reactivePowerSetpoint = reactivePowerSetpoint;
    return *this;
}

StaticVarCompensatorAdder& StaticVarCompensatorAdder::setRegulatingTerminal(const stdcxx::Reference<Terminal>& regulatingTerminal) {
    m_regulatingTerminal = regulatingTerminal;
    return *this;
}

StaticVarCompensatorAdder& StaticVarCompensatorAdder::setRegulationMode(const StaticVarCompensator::RegulationMode& regulationMode) {
    m_regulationMode = regulationMode;
    return *this;
}

StaticVarCompensatorAdder& StaticVarCompensatorAdder::setVoltageSetpoint(double voltageSetpoint) {
    m_voltageSetpoint = voltageSetpoint;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
