/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/HvdcConverterStation.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

HvdcConverterStation::HvdcConverterStation(const std::string& id, const std::string& name, bool fictitious, double lossFactor) :
    Injection(id, name, fictitious, ConnectableType::HVDC_CONVERTER_STATION),
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
    if (m_hvdcLine) {
        throw ValidationException(*this, stdcxx::format("Impossible to remove this converter station (still attached to '%1%')", m_hvdcLine.get().getId()));
    }
    Injection::remove();
}

void HvdcConverterStation::setHvdcLine(const stdcxx::Reference<HvdcLine>& hvdcLine) {
    m_hvdcLine = hvdcLine;
}

HvdcConverterStation& HvdcConverterStation::setLossFactor(double lossFactor) {
    m_lossFactor = checkLossFactor(*this, lossFactor);
    return *this;
}

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<HvdcConverterStation::HvdcType>() {
    static std::initializer_list<std::string> s_typeNames {
            "LCC",
            "VSC"
    };
    return s_typeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
