/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_PHASETAPCHANGERSTEPSREPLACER_HPP
#define POWSYBL_IIDM_PHASETAPCHANGERSTEPSREPLACER_HPP

#include <powsybl/iidm/PhaseTapChangerStep.hpp>
#include <powsybl/iidm/PhaseTapChangerStepAdder.hpp>
#include <powsybl/iidm/TapChangerStepsReplacer.hpp>
#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class PhaseTapChanger;

class PhaseTapChangerStepsReplacer : public TapChangerStepsReplacer<PhaseTapChanger, PhaseTapChangerStep, PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer>> {

public:
    explicit PhaseTapChangerStepsReplacer(PhaseTapChanger& ptc);

    ~PhaseTapChangerStepsReplacer() noexcept = default;

    //TapChangerStepsReplacer
    PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer> beginStep() override;

    void replaceSteps() override;

private:
    PhaseTapChangerStepsReplacer& addStep(double alpha, double rho, double r, double x, double g, double b);

    Validable& getValidable();

    friend class PhaseTapChangerStepAdder<PhaseTapChangerStepsReplacer>;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_PHASETAPCHANGERSTEPSREPLACER_HPP
