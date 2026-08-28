/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERSTEPSREPLACER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERSTEPSREPLACER_HPP

#include <powsybl/iidm/RatioTapChangerStep.hpp>
#include <powsybl/iidm/RatioTapChangerStepAdder.hpp>
#include <powsybl/iidm/TapChangerStepsReplacer.hpp>
#include <powsybl/iidm/Validable.hpp>

namespace powsybl {

namespace iidm {

class RatioTapChanger;

class RatioTapChangerStepsReplacer : public TapChangerStepsReplacer<RatioTapChanger, RatioTapChangerStep, RatioTapChangerStepAdder<RatioTapChangerStepsReplacer>> {

public:
    explicit RatioTapChangerStepsReplacer(RatioTapChanger& rtc);

    ~RatioTapChangerStepsReplacer() noexcept = default;

    //TapChangerStepsReplacer
    RatioTapChangerStepAdder<RatioTapChangerStepsReplacer> beginStep() override;

    void replaceSteps() override;

private:
    RatioTapChangerStepsReplacer& addStep(RatioTapChangerStep&& step);

    Validable& getValidable();

    friend class RatioTapChangerStepAdder<RatioTapChangerStepsReplacer>;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERSTEPSREPLACER_HPP
