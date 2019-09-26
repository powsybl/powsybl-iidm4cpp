/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/PhaseTapChangerStep.hpp>

namespace powsybl {

namespace iidm {

PhaseTapChangerStep::PhaseTapChangerStep(unsigned long position, double alpha, double rho, double r, double x, double g, double b) :
    TapChangerStep(position, rho, r, x, g, b),
    m_alpha(alpha) {
}

double PhaseTapChangerStep::getAlpha() const {
    return m_alpha;
}

PhaseTapChangerStep& PhaseTapChangerStep::setAlpha(double alpha) {
    m_alpha = alpha;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
