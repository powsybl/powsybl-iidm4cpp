/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HXX
#define POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HXX

#include <powsybl/iidm/RatioTapChangerStepAdder.hpp>

namespace powsybl {

namespace iidm {

template<typename Parent>
RatioTapChangerStepAdder<Parent>& RatioTapChangerStepAdder<Parent>::setB(double b) {
    TapChangerStepAdder<RatioTapChangerStepAdder<Parent>, Parent>::m_b = b;
    return *this;
}

template<typename Parent>
RatioTapChangerStepAdder<Parent>& RatioTapChangerStepAdder<Parent>::setG(double g) {
    TapChangerStepAdder<RatioTapChangerStepAdder<Parent>, Parent>::m_g = g;
    return *this;
}

template<typename Parent>
RatioTapChangerStepAdder<Parent>& RatioTapChangerStepAdder<Parent>::setR(double r) {
    TapChangerStepAdder<RatioTapChangerStepAdder<Parent>, Parent>::m_r = r;
    return *this;
}

template<typename Parent>
RatioTapChangerStepAdder<Parent>& RatioTapChangerStepAdder<Parent>::setRho(double rho) {
    TapChangerStepAdder<RatioTapChangerStepAdder<Parent>, Parent>::m_rho = rho;
    return *this;
}

template<typename Parent>
RatioTapChangerStepAdder<Parent>& RatioTapChangerStepAdder<Parent>::setX(double x) {
    TapChangerStepAdder<RatioTapChangerStepAdder<Parent>, Parent>::m_x = x;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HXX
