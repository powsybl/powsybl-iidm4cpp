/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HPP
#define POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HPP

#include <powsybl/iidm/TapChangerStepAdder.hpp>

namespace powsybl {

namespace iidm {

class PhaseTapChangerAdder;

class PhaseTapChangerStepAdder : public TapChangerStepAdder<PhaseTapChangerStepAdder, PhaseTapChangerAdder> {
public:
    ~PhaseTapChangerStepAdder() noexcept = default;

    PhaseTapChangerStepAdder& setRho(double rho) override;

    PhaseTapChangerStepAdder& setR(double r) override;

    PhaseTapChangerStepAdder& setX(double x) override;

    PhaseTapChangerStepAdder& setG(double g) override;

    PhaseTapChangerStepAdder& setB(double b) override;

    PhaseTapChangerAdder& endStep() override;

    PhaseTapChangerStepAdder& setAlpha(double alpha);

private:
    explicit PhaseTapChangerStepAdder(PhaseTapChangerAdder &parent);
    friend class PhaseTapChangerAdder;

    double m_alpha = stdcxx::nan();
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERSTEPADDER_HPP
