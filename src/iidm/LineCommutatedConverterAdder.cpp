/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LineCommutatedConverterAdder.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

LineCommutatedConverterAdder::LineCommutatedConverterAdder(VoltageLevel& voltageLevel) :
    AcDcConverterAdder(voltageLevel) {

}

const std::string& LineCommutatedConverterAdder::getTypeDescription() const {
    static std::string s_typeDescription = "AC/DC Line Commutated Converter";;
    return s_typeDescription;
}

LineCommutatedConverter& LineCommutatedConverterAdder::add() {
    std::string id = checkAndGetUniqueId();
    checkAcDcConverter();
    checkPowerFactorPositive(*this, m_powerFactor);
    checkLccReactiveModel(*this, m_reactiveModel);

    auto ptrConverter = std::unique_ptr<LineCommutatedConverter>(new LineCommutatedConverter(id, getName(), isFictitious(), getNetwork().getVariantManager().getVariantArraySize(), 
                    m_idleLoss, m_switchingLoss, m_resistiveLoss, m_pccTerminal, m_controlMode, m_targetP, m_targetVdc, 
                    m_reactiveModel, m_powerFactor));

    auto& converter = checkAndAdd(std::move(ptrConverter));
    return converter;
}

LineCommutatedConverterAdder& LineCommutatedConverterAdder::setReactiveModel(const LineCommutatedConverter::ReactiveModel& reactiveModel) {
    m_reactiveModel = reactiveModel;
    return *this;
}

LineCommutatedConverterAdder& LineCommutatedConverterAdder::setPowerFactor(double powerFactor) {
    m_powerFactor = powerFactor;
    return *this;
}




}  // namespace iidm

}  // namespace powsybl

