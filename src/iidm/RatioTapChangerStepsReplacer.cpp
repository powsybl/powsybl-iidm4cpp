/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChangerStepsReplacer.hpp>
#include <powsybl/iidm/RatioTapChangerStepAdder.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

RatioTapChangerStepsReplacer::RatioTapChangerStepsReplacer(RatioTapChanger& rtc)  :
    TapChangerStepsReplacer(rtc) {
}


RatioTapChangerStepAdder<RatioTapChangerStepsReplacer> RatioTapChangerStepsReplacer::beginStep() {
    return RatioTapChangerStepAdder<RatioTapChangerStepsReplacer>(*this);
}

RatioTapChangerStepsReplacer& RatioTapChangerStepsReplacer::addStep(double rho, double r, double x, double g, double b) {
    m_newSteps.emplace_back(rho, r, x, g, b);
    return *this;
}

void RatioTapChangerStepsReplacer::replaceSteps() {
    m_tapChangerParent.setSteps(m_newSteps);
}

Validable& RatioTapChangerStepsReplacer::getValidable() {
    return m_tapChangerParent.getParent();
}

}  // namespace iidm

}  // namespace powsybl
