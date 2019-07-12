/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LccConverterStation.hpp>
#include <powsybl/iidm/LccConverterStationAdder.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

LccConverterStationAdder::LccConverterStationAdder(VoltageLevel& voltageLevel) :
    HvdcConverterStationAdder(voltageLevel),
    m_powerFactor(stdcxx::nan()) {
}

LccConverterStation& LccConverterStationAdder::add() {
    validate();

    std::unique_ptr<LccConverterStation> ptrLcc = stdcxx::make_unique<LccConverterStation>(getId(), getName(), getLossFactor(), m_powerFactor);
    auto& lcc = getNetwork().checkAndAdd<LccConverterStation>(std::move(ptrLcc));

    Terminal& terminal = lcc.addTerminal(checkAndGetTerminal());
    getVoltageLevel().attach(terminal, false);

    return lcc;
}

const std::string& LccConverterStationAdder::getTypeDescription() const {
    static std::string s_typeDescription = "lccConverterStation";

    return s_typeDescription;
}

LccConverterStationAdder& LccConverterStationAdder::setPowerFactor(double powerFactor) {
    m_powerFactor = powerFactor;
    return *this;
}

void LccConverterStationAdder::validate() const {
    HvdcConverterStationAdder::validate();
    checkPowerFactor(*this, m_powerFactor);
}

}  // namespace iidm

}  // namespace powsybl
