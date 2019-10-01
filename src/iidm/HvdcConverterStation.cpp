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

void HvdcConverterStation::remove() {
    if (static_cast<bool>(m_hvdcLine)) {
        throw ValidationException(*this, logging::format("Impossible to remove this converter station (still attached to '%1%')", m_hvdcLine.get().getId()));
    }
    Injection::remove();
}

void HvdcConverterStation::resetHvdcLine() {
    m_hvdcLine.reset();
}

HvdcConverterStation& HvdcConverterStation::setHvdcLine(HvdcLine& hvdcLine) {
    m_hvdcLine = stdcxx::ref<HvdcLine>(hvdcLine);
    return *this;
}

void HvdcConverterStation::setLossFactor(double lossFactor) {
    m_lossFactor = checkLossFactor(*this, lossFactor);
}

}  // namespace iidm

}  // namespace powsybl
