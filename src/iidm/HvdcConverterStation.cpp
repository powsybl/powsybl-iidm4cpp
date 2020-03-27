/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HvdcConverterStation.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

HvdcConverterStation::HvdcConverterStation(const std::string& id, const std::string& name, double lossFactor) :
    Injection(id, name, ConnectableType::HVDC_CONVERTER_STATION),
    m_lossFactor(checkLossFactor(*this, lossFactor)) {

}

stdcxx::CReference<HvdcLine> HvdcConverterStation::getHvdcLine() const {
    return stdcxx::cref(m_hvdcLine);
}

stdcxx::Reference<HvdcLine> HvdcConverterStation::getHvdcLine() {
    return m_hvdcLine;
}

double HvdcConverterStation::getLossFactor() const {
    return m_lossFactor;
}

void HvdcConverterStation::setHvdcLine(const stdcxx::Reference<HvdcLine>& hvdcLine) {
    m_hvdcLine = hvdcLine;
}

void HvdcConverterStation::setLossFactor(double lossFactor) {
    m_lossFactor = checkLossFactor(*this, lossFactor);
}

}  // namespace iidm

}  // namespace powsybl
