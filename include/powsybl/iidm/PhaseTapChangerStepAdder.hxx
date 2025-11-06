/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HXX
#define POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HXX

#include <powsybl/iidm/PhaseTapChangerStepAdder.hpp>

namespace powsybl {

namespace iidm {

template<typename Parent>
PhaseTapChangerStepAdder<Parent>& PhaseTapChangerStepAdder<Parent>::setB(double b) {
    TapChangerStepAdder<PhaseTapChangerStepAdder<Parent>, Parent>::m_b = b;
    return *this;
}

template<typename Parent>
PhaseTapChangerStepAdder<Parent>& PhaseTapChangerStepAdder<Parent>::setG(double g) {
    TapChangerStepAdder<PhaseTapChangerStepAdder<Parent>, Parent>::m_g = g;
    return *this;
}

template<typename Parent>
PhaseTapChangerStepAdder<Parent>& PhaseTapChangerStepAdder<Parent>::setR(double r) {
    TapChangerStepAdder<PhaseTapChangerStepAdder<Parent>, Parent>::m_r = r;
    return *this;
}

template<typename Parent>
PhaseTapChangerStepAdder<Parent>& PhaseTapChangerStepAdder<Parent>::setRho(double rho) {
    TapChangerStepAdder<PhaseTapChangerStepAdder<Parent>, Parent>::m_rho = rho;
    return *this;
}

template<typename Parent>
PhaseTapChangerStepAdder<Parent>& PhaseTapChangerStepAdder<Parent>::setX(double x) {
    TapChangerStepAdder<PhaseTapChangerStepAdder<Parent>, Parent>::m_x = x;
    return *this;
}

template<typename Parent>
PhaseTapChangerStepAdder<Parent>& PhaseTapChangerStepAdder<Parent>::setAlpha(double alpha) {
    m_alpha = alpha;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HXX
