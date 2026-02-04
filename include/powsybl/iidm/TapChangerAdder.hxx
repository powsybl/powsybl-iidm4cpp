/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERADDER_HXX
#define POWSYBL_IIDM_TAPCHANGERADDER_HXX

#include <powsybl/iidm/TapChangerAdder.hpp>

namespace powsybl {

namespace iidm {

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::TapChangerAdder(TCHolder& parent) :
    m_parent(parent) {
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
Network& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::getNetwork() {
    return m_parent.getNetwork();
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TCAdder& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::setLowTapPosition(long lowTapPosition) {
    m_lowTapPosition = lowTapPosition;
    return static_cast<TCAdder&>(*this);
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TCAdder& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::setTapPosition(long tapPosition) {
    m_tapPosition = tapPosition;
    return static_cast<TCAdder&>(*this);
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TCAdder& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::setRegulating(bool regulating) {
    m_regulating = regulating;
    return static_cast<TCAdder&>(*this);
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TCAdder& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::setRegulationTerminal(const stdcxx::Reference<Terminal>& regulationTerminal) {
    m_regulationTerminal = regulationTerminal;
    return static_cast<TCAdder&>(*this);
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TCAdder& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::setTargetDeadband(double targetDeadband) {
    m_targetDeadband = targetDeadband;
    return static_cast<TCAdder&>(*this);
}

template<typename TC, typename TCAdder, typename TCStepAdder, typename TCHolder>
TCAdder& TapChangerAdder<TC, TCAdder, TCStepAdder, TCHolder>::setRegulationValue(double regulationValue) {
    m_regulationValue = regulationValue;
    return static_cast<TCAdder&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGERADDER_HXX
