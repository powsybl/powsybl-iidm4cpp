/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChangerStepsReplacer.hpp>
#include <powsybl/iidm/PhaseTapChangerStepAdder.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

PhaseTapChangerStepsReplacer::PhaseTapChangerStepsReplacer(PhaseTapChanger& ptc)  :
    TapChangerStepsReplacer(ptc) {
}


PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer> PhaseTapChangerStepsReplacer::beginStep() {
    return PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer>(*this);
}

PhaseTapChangerStepsReplacer& PhaseTapChangerStepsReplacer::addStep(double alpha, double rho, double r, double x, double g, double b) {
    m_newSteps.emplace_back(alpha, rho, r, x, g, b);
    return *this;
}

void PhaseTapChangerStepsReplacer::replaceSteps() {
    m_tapChangerParent.setSteps(m_newSteps);
}

Validable& PhaseTapChangerStepsReplacer::getValidable() {
    return m_tapChangerParent.getParent();
}

}  // namespace iidm

}  // namespace powsybl
