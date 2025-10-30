/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChangerStepAdder.hpp>

#include <powsybl/iidm/PhaseTapChangerAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

PhaseTapChangerStepAdder::PhaseTapChangerStepAdder(PhaseTapChangerAdder& parent) :
    TapChangerStepAdder(parent) {
    m_rho = 1.0;
}

PhaseTapChangerStepAdder& PhaseTapChangerStepAdder::setB(double b) {
    m_b = b;
    return *this;
}

PhaseTapChangerStepAdder& PhaseTapChangerStepAdder::setG(double g) {
    m_g = g;
    return *this;
}

PhaseTapChangerStepAdder& PhaseTapChangerStepAdder::setR(double r) {
    m_r = r;
    return *this;
}

PhaseTapChangerStepAdder& PhaseTapChangerStepAdder::setRho(double rho) {
    m_rho = rho;
    return *this;
}

PhaseTapChangerStepAdder& PhaseTapChangerStepAdder::setX(double x) {
    m_x = x;
    return *this;
}

PhaseTapChangerStepAdder& PhaseTapChangerStepAdder::setAlpha(double alpha) {
    m_alpha = alpha;
    return *this;
}

PhaseTapChangerAdder& PhaseTapChangerStepAdder::endStep() {
    checkOptional(m_parent.getValidable(), m_alpha, "step alpha is not set");
    checkOptional(m_parent.getValidable(), m_rho, "step rho is not set");
    checkOptional(m_parent.getValidable(), m_r, "step r is not set");
    checkOptional(m_parent.getValidable(), m_x, "step x is not set");
    checkOptional(m_parent.getValidable(), m_g, "step g is not set");
    checkOptional(m_parent.getValidable(), m_b, "step b is not set");

    return m_parent.addStep(m_alpha, m_rho, m_r, m_x, m_g, m_b);
}

}  // namespace iidm

}  // namespace powsybl
