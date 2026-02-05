/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HXX
#define POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HXX

#include <powsybl/iidm/HvdcConverterStationAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

template<typename Added, typename Adder>
HvdcConverterStationAdder<Added, Adder>::HvdcConverterStationAdder(VoltageLevel& voltageLevel) :
    InjectionAdder<Added, Adder>(voltageLevel) {
}

template<typename Added, typename Adder>
double HvdcConverterStationAdder<Added, Adder>::getLossFactor() const {
    return m_lossFactor;
}

template<typename Added, typename Adder>
Adder& HvdcConverterStationAdder<Added, Adder>::setLossFactor(double lossFactor) {
    m_lossFactor = lossFactor;
    return static_cast<Adder&>(*this);
}

template<typename Added, typename Adder>
void HvdcConverterStationAdder<Added, Adder>::validate() {
    checkLossFactor(*this, m_lossFactor, InjectionAdder<Added, Adder>::getNetwork().getMinimumValidationLevel());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_HVDCCONVERTERSTATIONADDER_HXX
