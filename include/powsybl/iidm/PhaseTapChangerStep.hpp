/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERSTEP_HPP
#define POWSYBL_IIDM_PHASETAPCHANGERSTEP_HPP

#include <powsybl/iidm/TapChangerStep.hpp>

namespace powsybl {

namespace iidm {

class PhaseTapChangerStep : public TapChangerStep<PhaseTapChangerStep> {
public:
    PhaseTapChangerStep(double alpha, double rho, double r, double x, double g, double b);

    ~PhaseTapChangerStep() noexcept override = default;

    double getAlpha() const;

    PhaseTapChangerStep& setAlpha(double alpha);

private:
    double m_alpha;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERSTEP_HPP
