/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChangerStepAdder.hpp>

#include <powsybl/iidm/RatioTapChangerAdder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

RatioTapChangerStepAdder::RatioTapChangerStepAdder(RatioTapChangerAdder& parent) :
    TapChangerStepAdder(parent) {
}

RatioTapChangerStepAdder& RatioTapChangerStepAdder::setB(double b) {
    m_b = b;
    return *this;
}

RatioTapChangerStepAdder& RatioTapChangerStepAdder::setG(double g) {
    m_g = g;
    return *this;
}

RatioTapChangerStepAdder& RatioTapChangerStepAdder::setR(double r) {
    m_r = r;
    return *this;
}

RatioTapChangerStepAdder& RatioTapChangerStepAdder::setRho(double rho) {
    m_rho = rho;
    return *this;
}

RatioTapChangerStepAdder& RatioTapChangerStepAdder::setX(double x) {
    m_x = x;
    return *this;
}

RatioTapChangerAdder& RatioTapChangerStepAdder::endStep() {
    checkOptional(m_parent.getValidable(), m_rho, "step rho is not set");
    checkOptional(m_parent.getValidable(), m_r, "step r is not set");
    checkOptional(m_parent.getValidable(), m_x, "step x is not set");
    checkOptional(m_parent.getValidable(), m_g, "step g is not set");
    checkOptional(m_parent.getValidable(), m_b, "step b is not set");

    return m_parent.addStep(m_rho, m_r, m_x, m_g, m_b);
}

}  // namespace iidm

}  // namespace powsybl
