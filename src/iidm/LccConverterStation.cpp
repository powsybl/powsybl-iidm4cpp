/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LccConverterStation.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

LccConverterStation::LccConverterStation(const std::string& id, const std::string& name, double lossFactor, double powerFactor) :
    HvdcConverterStation(id, name, lossFactor),
    m_powerFactor(checkPowerFactor(*this, powerFactor)) {
}

HvdcConverterStation::HvdcType LccConverterStation::getHvdcType() const {
    return HvdcType::LCC;
}

double LccConverterStation::getPowerFactor() const {
    return m_powerFactor;
}

const std::string& LccConverterStation::getTypeDescription() const {
    static std::string s_typeDescription = "lccConverterStation";

    return s_typeDescription;
}

LccConverterStation& LccConverterStation::setLossFactor(double lossFactor) {
    HvdcConverterStation::setLossFactor(lossFactor);
    return *this;
}

LccConverterStation& LccConverterStation::setPowerFactor(double powerFactor) {
    m_powerFactor = checkPowerFactor(*this, powerFactor);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
