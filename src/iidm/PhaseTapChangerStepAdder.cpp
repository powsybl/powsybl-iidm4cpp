/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChangerStepAdder.hpp>

#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/PhaseTapChangerStepsReplacer.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

template<>
PhaseTapChangerStepAdder<PhaseTapChangerAdder>::PhaseTapChangerStepAdder(PhaseTapChangerAdder& parent) :
    TapChangerStepAdder(parent) {
    m_rho = 1.0;
}

template<>
PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer>::PhaseTapChangerStepAdder(PhaseTapChangerStepsReplacer& parent) :
    TapChangerStepAdder(parent) {
    m_rho = 1.0;
}

template<>
PhaseTapChangerAdder& PhaseTapChangerStepAdder<PhaseTapChangerAdder>::endStep() {
    checkOptional(m_parent.getValidable(), m_alpha, "step alpha is not set");
    checkOptional(m_parent.getValidable(), m_rho, "step rho is not set");
    checkOptional(m_parent.getValidable(), m_r, "step r is not set");
    checkOptional(m_parent.getValidable(), m_x, "step x is not set");
    checkOptional(m_parent.getValidable(), m_g, "step g is not set");
    checkOptional(m_parent.getValidable(), m_b, "step b is not set");

    PhaseTapChangerStep step = PhaseTapChangerStep(m_alpha, m_rho, m_r, m_x, m_g, m_b);
    copyPropertiesTo(step);
    return m_parent.addStep(std::move(step));
}

template<>
PhaseTapChangerStepsReplacer& PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer>::endStep() {
    checkOptional(m_parent.getValidable(), m_alpha, "step alpha is not set");
    checkOptional(m_parent.getValidable(), m_rho, "step rho is not set");
    checkOptional(m_parent.getValidable(), m_r, "step r is not set");
    checkOptional(m_parent.getValidable(), m_x, "step x is not set");
    checkOptional(m_parent.getValidable(), m_g, "step g is not set");
    checkOptional(m_parent.getValidable(), m_b, "step b is not set");

    PhaseTapChangerStep step = PhaseTapChangerStep(m_alpha, m_rho, m_r, m_x, m_g, m_b);
    copyPropertiesTo(step);
    return m_parent.addStep(std::move(step));
}

}  // namespace iidm

}  // namespace powsybl
