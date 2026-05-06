/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/VoltageSourceConverterAdder.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

VoltageSourceConverterAdder::VoltageSourceConverterAdder(VoltageLevel& voltageLevel) :
    AcDcConverterAdder(voltageLevel) {

}

const std::string& VoltageSourceConverterAdder::getTypeDescription() const {
    static std::string s_typeDescription =  "AC/DC Voltage Source Converter";
    return s_typeDescription;
}

VoltageSourceConverter& VoltageSourceConverterAdder::add() {
    std::string id = checkAndGetUniqueId();
    checkAcDcConverter();
    Network& network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkVoltageControl(*this, m_voltageRegulatorOn, m_voltageSetpoint, m_reactivePowerSetpoint, network.getMinimumValidationLevel()));
    checkRegulatingTerminal(*this, m_pccTerminal, network);

    auto ptrConverter = std::unique_ptr<VoltageSourceConverter>(new VoltageSourceConverter(id, getName(), isFictitious(), network.getVariantManager().getVariantArraySize(), 
                    m_idleLoss, m_switchingLoss, m_resistiveLoss, m_pccTerminal, m_controlMode, m_targetP, m_targetVdc, 
                    m_voltageRegulatorOn, m_reactivePowerSetpoint, m_voltageSetpoint));

    auto& converter = checkAndAdd(std::move(ptrConverter));
    return converter;
}

VoltageSourceConverterAdder& VoltageSourceConverterAdder::setVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn = voltageRegulatorOn;
    return *this;
}

VoltageSourceConverterAdder& VoltageSourceConverterAdder::setVoltageSetpoint(double voltageSetpoint) {
    m_voltageSetpoint = voltageSetpoint;
    return *this;
}

VoltageSourceConverterAdder& VoltageSourceConverterAdder::setReactivePowerSetpoint(double reactivePowerSetpoint) {
    m_reactivePowerSetpoint = reactivePowerSetpoint;
    return *this;
}




}  // namespace iidm

}  // namespace powsybl
