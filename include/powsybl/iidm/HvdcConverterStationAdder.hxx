/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HXX
#define POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HXX

#include <powsybl/iidm/HvdcConverterStationAdder.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

double checkLossFactor(const Validable& validable, double lossFactor);

template<typename Adder>
HvdcConverterStationAdder<Adder>::HvdcConverterStationAdder(VoltageLevel& voltageLevel) :
    m_voltageLevel(voltageLevel),
    m_lossFactor(stdcxx::nan()) {
}

template<typename Adder>
double HvdcConverterStationAdder<Adder>::getLossFactor() const {
    return m_lossFactor;
}

template<typename Adder>
Network& HvdcConverterStationAdder<Adder>::getNetwork() {
    return m_voltageLevel.getNetwork();
}

template<typename Adder>
VoltageLevel& HvdcConverterStationAdder<Adder>::getVoltageLevel() const {
    return m_voltageLevel;
}

template<typename Adder>
Adder& HvdcConverterStationAdder<Adder>::setLossFactor(double lossFactor) {
    m_lossFactor = lossFactor;
    return static_cast<Adder&>(*this);
}

template<typename Adder>
void HvdcConverterStationAdder<Adder>::validate() const {
    checkLossFactor(*this, m_lossFactor);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HXX
